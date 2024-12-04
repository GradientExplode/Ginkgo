#include <gkg-core-object/BinaryHeader.h>
#include <gkg-core-exception/Exception.h>

#include <algorithm>
#include <functional>
#include <cstring>


gkg::BinaryHeader::BinaryHeader()
                 : _format( "" )
{
}


gkg::BinaryHeader::BinaryHeader( const gkg::BinaryHeader& other )
{

  this->operator = ( other );

}


gkg::BinaryHeader::~BinaryHeader()
{

  clear();

}


gkg::BinaryHeader& gkg::BinaryHeader::operator = ( const gkg::BinaryHeader& other )
{

  try
  {

    resize( other.getCount() );

    uint32_t i = 0;
    std::vector< gkg::BinaryHeader::Buffer >::const_iterator
      o = other.begin(),
      oe = other.end();
    std::vector< gkg::BinaryHeader::Buffer >::iterator
      b = _buffers.begin();

    while ( o != oe )
    {

      if ( allocate( i, o->second ) )
      {

        std::memcpy( b->first, o->first, o->second );
        b->second = o->second;

      }

      ++o;
      ++b;
      i++;

    }

    _lut = other._lut;
    _format = other._format;

    return *this;

  }
  GKG_CATCH( "gkg::BinaryHeader& gkg::BinaryHeader::operator = ( "
             "const gkg::BinaryHeader& other )" );

}


gkg::BinaryHeader::Buffer& gkg::BinaryHeader::operator [] ( const uint32_t index )
{

  try
  {

    return _buffers[ _lut[ index ] ];

  }
  GKG_CATCH( "gkg::BinaryHeader::Buffer& gkg::BinaryHeader::operator [] ( "
             "const uint32_t index )" );

}


gkg::BinaryHeader::iterator gkg::BinaryHeader::begin()
{

  return _buffers.begin();

}


gkg::BinaryHeader::iterator gkg::BinaryHeader::end()
{

  return _buffers.end();

}


gkg::BinaryHeader::const_iterator gkg::BinaryHeader::begin() const
{

  return _buffers.begin();

}


gkg::BinaryHeader::const_iterator gkg::BinaryHeader::end() const
{

  return _buffers.end();

}


int32_t gkg::BinaryHeader::getCount() const
{

  return (int32_t)_buffers.size();

}


std::vector< int32_t >& gkg::BinaryHeader::getLut()
{

  return _lut;

}


void gkg::BinaryHeader::resize( uint32_t n, gkg::BinaryHeader::Buffer buffer )
{

  if ( n )
  {

    _buffers.resize( n, buffer );
    _lut.resize( n, 0 );

    int32_t i = 0;
    std::vector< int32_t >::iterator
      l = _lut.begin(),
      le = _lut.end();

    while ( l != le )
    {

      *l = i++;
      ++l;

    }

  }

}


bool gkg::BinaryHeader::allocate( uint32_t index, uint32_t length )
{

  if ( index < uint32_t( _buffers.size() ) )
  {

    gkg::BinaryHeader::Buffer& buffer = _buffers[ index ];

    return allocate( buffer, length );

  }

  return false;

}


bool gkg::BinaryHeader::allocate( gkg::BinaryHeader::Buffer& buffer, 
                                  uint32_t length )
{

  if ( length )
  {

    if ( buffer.first )
    {

      delete [] buffer.first;

    }

    buffer.first = new uint8_t[ length ];

    if ( buffer.first )
    {

      buffer.second = length;

      return true;

    }

  }

  return false;

}


void gkg::BinaryHeader::add( gkg::BinaryHeader::Buffer& buffer )
{

  _buffers.push_back( buffer );
  _lut.push_back( int32_t( _lut.size() ) );

}


void gkg::BinaryHeader::trim()
{

  std::vector< gkg::BinaryHeader::Buffer >::iterator
    last = std::remove_if( _buffers.begin(),
                           _buffers.end(),
                           std::bind2nd( 
                                   std::equal_to< gkg::BinaryHeader::Buffer >(),
                                   gkg::BinaryHeader::Buffer( 0, 0 ) ) );

  _buffers.erase( last, _buffers.end() );
  _lut.resize( _buffers.size() );

  int32_t i = 0;
  std::vector< int32_t >::iterator
    l = _lut.begin(),
    le = _lut.end();

  while ( l != le )
  {

    *l = i++;
    ++l;

  }

}


void gkg::BinaryHeader::clear()
{

  std::vector< gkg::BinaryHeader::Buffer >::iterator
    b = _buffers.begin(),
    be = _buffers.end();

  while ( b != be )
  {

    delete [] b->first;
    ++b;

  }

  _buffers.clear();
  _lut.clear();
  _format.clear();

}


void gkg::BinaryHeader::setFormat( const std::string& format )
{

  _format = format;

}


const std::string& gkg::BinaryHeader::getFormat() const
{

  return _format;

}

