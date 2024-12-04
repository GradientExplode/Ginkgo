#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <climits>
#include <cassert>

//
// FilteredInputStreamBuffer methods
//


template < class F >
gkg::FilteredInputStreamBuffer< F >::FilteredInputStreamBuffer(
                                                         std::streambuf* source,
                                                         const F& filter,
                                                         bool autodelete )
                                    : _source( source ),
                                      _filter( filter ),
                                      _autodelete( autodelete )
{


}


template < class F >
gkg::FilteredInputStreamBuffer< F >::FilteredInputStreamBuffer(
                                                         std::streambuf* source,
                                                         bool autodelete )
                                    : _source( source ),
                                      _autodelete( autodelete )
{


}


template < class F >
gkg::FilteredInputStreamBuffer< F >::~FilteredInputStreamBuffer()
{

  sync();
  if ( _autodelete )
  {

    delete _source;

  }

}


template < class F >
F& gkg::FilteredInputStreamBuffer< F >::getFilter()
{

  return _filter;

}


template < class F >
int gkg::FilteredInputStreamBuffer< F >::underflow()
{

  int result = EOF;
  if ( gptr() < egptr() )
  {

    result = *gptr();

  }
  else if ( _source )
  {

    result = _filter( *_source );
    if ( result != EOF )
    {

      assert( ( result >= 0 ) && ( result <= UCHAR_MAX ) );
      _buffer = result;
      setg( &_buffer , &_buffer , &_buffer + 1 );

    }

  }
  return result;

}


template < class F >
int gkg::FilteredInputStreamBuffer< F >::sync()
{

  int result = 0;
  if ( _source )
  {

    if ( gptr() < egptr() )
    {

      result = _source->sputbackc( *gptr() );
      setg(0, 0, 0);

    }
#if defined (__GNUC__) && (__GNUC__-0 < 3)

    if ( _source->sync() == EOF )

#else

    if ( _source->pubsync() == EOF )

#endif
    {

      result = EOF;

    }

  }
  return result;

}


template < class F >
std::streambuf* gkg::FilteredInputStreamBuffer< F >::setbuf( char* p,
                                                             int length )
{

#if defined (__GNUC__) && (__GNUC__-0 < 3)

  return _source ? _source->setbuf( p, length ) : 0;

#else

  return _source ? _source->pubsetbuf( p, length ) : 0;

#endif

}


//
// UncommentFilter methods
//

gkg::UncommentFilter::UncommentFilter( char commentCharacter )
                     : _commentCharacter( commentCharacter )
{
}


int gkg::UncommentFilter::operator()( std::streambuf& source )
{

  int c = source.sbumpc();
  if ( c == _commentCharacter )
  {

    while ( ( c != '\n' ) && ( c != EOF ) )
    {

      c = source.sbumpc();

    }

  }
  return c;

}


//
// CounterFilter methods
//

gkg::CounterFilter::CounterFilter( int line )
                   : _line( line ),
                     _newLine( false )
{

}

int gkg::CounterFilter::operator()( std::streambuf& source )
{

  int c = source.sbumpc();
  if ( _newLine )
  {

    ++_line;
    _newLine = false;

  }
  if ( c == '\n' )
  {

    _newLine = true;

  }
  return c;

}


int gkg::CounterFilter::getLine() const
{

  return _line;

}


int gkg::CounterFilter::reset( int line )
{

  int tmp = _line;
  _line = line;
  return tmp;

}


//
// UncommentCounterInputFileStream methods
//

gkg::UncommentCounterInputFileStream::UncommentCounterInputFileStream()
                                     : std::istream( 0 ),
                                       _uncomment( 0 ),
                                       _counter( 0 )
{
}


gkg::UncommentCounterInputFileStream::UncommentCounterInputFileStream(
                                                              const char* name )
                                     : std::istream( 0 ),
                                       _base( name ),
                                       _uncomment( 0 ),
                                       _counter( 0 )
{

  init_buffers();

}


gkg::UncommentCounterInputFileStream::~UncommentCounterInputFileStream()
{

  destroy_buffers();

}


void gkg::UncommentCounterInputFileStream::open( const char* name )
{

  destroy_buffers();
  std::istream::clear();
  _base.clear();
  _base.open( name );
  init_buffers();

}


void gkg::UncommentCounterInputFileStream::close()
{

  _base.close();
  destroy_buffers();

}


bool gkg::UncommentCounterInputFileStream::operator!() const
{

  return _base.operator!() || std::istream::operator!();

}


bool gkg::UncommentCounterInputFileStream::is_open() const
{

  return _base.is_open();

}


#if defined (__GNUC__) && (__GNUC__-0 < 3)

void gkg::UncommentCounterInputFileStream::specialSeekg(
                                                        streamoff offset,
                                                        std::ios::seek_dir dir )

#else

void gkg::UncommentCounterInputFileStream::specialSeekg(
                                                  std::istream::off_type offset,
                                                  std::ios_base::seekdir dir )

#endif
{

  _base.seekg( offset, dir );

}


int gkg::UncommentCounterInputFileStream::getLine() const
{

  return _counter->getFilter().getLine();

}


void gkg::UncommentCounterInputFileStream::init_buffers()
{

  rdbuf( _base.rdbuf() );
  _counter = new gkg::FilteredInputStreamBuffer< gkg::CounterFilter >(
                                                              rdbuf() );
  rdbuf( _counter );
  _uncomment = new gkg::FilteredInputStreamBuffer< gkg::UncommentFilter>( 
                                                                 rdbuf() );
  rdbuf( _uncomment );

}


void gkg::UncommentCounterInputFileStream::destroy_buffers()
{

  if ( _uncomment )
  {

    delete _uncomment;
    _uncomment = 0;

  }
  if ( _counter )
  {

    delete _counter;
    _counter = 0;

  }

}



