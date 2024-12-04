#include <gkg-graphics-X11-core/TextBuffer.h>
#include <gkg-core-string/RegularExpression.h>
#include <gkg-core-cppext/CUniStd.h>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h> 
#include <sys/stat.h>


//
// local variables and functions
//

static const char NEWLINE = '\012';


static int32_t limit( int32_t low, int32_t x, int32_t high )
{

  return x < low ? low : x > high ? high : x;

}


//
// class TextBuffer
//

gkg::TextBuffer::TextBuffer( const std::string& text )
                : _text( text ),
                  _path( "" ),
                  _modified( false ),
                  _currentRegularExpression( 0 )
{

  _lineCount = 1 + getLinesBetween( 0, ( int32_t )_text.length() );
  _lastLine = 0;
  _lastIndex = 0;

}


gkg::TextBuffer::~TextBuffer()
{
}


gkg::TextBuffer::IOResult gkg::TextBuffer::load( const std::string& path )
{

  _path = path;

  // open file
  int32_t fd;
  fd = open( _path.c_str(), O_RDONLY);
  if ( fd < 0 )
  {

    return gkg::TextBuffer::OpenError; 

  }

  // get file size
  struct stat info;
  if ( fstat( fd, &info ) < 0 )
  {

    close( fd );
    return gkg::TextBuffer::OpenError;

  }
  int32_t length = ( int32_t )info.st_size;

  // allocate buffer
  char* buffer = new char[ length ];
  if ( !buffer )
  {

    close( fd );
    return gkg::TextBuffer::MemoryError;

  }

  // read file into buffer
  int32_t bytesRead = read( fd, buffer, length );
  if ( bytesRead != length )
  {

    close( fd );
    return gkg::TextBuffer::ReadError;

  }
  insert( 0, std::string( buffer ) );

  close(fd);
  return gkg::TextBuffer::ReadOk;

}


gkg::TextBuffer::IOResult gkg::TextBuffer::save()
{

  int32_t fd = open( ( char* )_path.c_str(), O_WRONLY | O_CREAT, 0666 );
  if ( fd < 0 )
  {

    return gkg::TextBuffer::OpenError;

  }

  int32_t length = write( fd, _text.c_str(), _text.length() );
  if ( length != ( int32_t )_text.length() )
  {

    perror( "gkg::TextBuffer:save" );
    return gkg::TextBuffer::WriteError;

  }
  return gkg::TextBuffer::WriteOk;

}


gkg::TextBuffer::IOResult gkg::TextBuffer::save( const std::string& path )
{

  _path = path;
  return save();

}


int32_t gkg::TextBuffer::search( gkg::RegularExpression* regularExpression,
                                 int32_t index, int32_t range, 
                                 int32_t stop ) const
{

  int32_t s = limit( 0, stop, _text.length() );
  int32_t i = limit( 0, index, s );
  return regularExpression->search( _text.substr( 0, s ), i, range );
}


int32_t gkg::TextBuffer::forwardSearch(
                                   gkg::RegularExpression* regularExpression,
                                   int32_t index ) const
{

  int32_t i = limit( 0, index, _text.length() );
  int32_t r = regularExpression->search( _text, i, _text.length() - i );
  if ( r >= 0 )
  {

    return regularExpression->getEndOfMatch();

  }
  else
  {

    return r;

  }

}


int32_t gkg::TextBuffer::backwardSearch( 
                                     gkg::RegularExpression* regularExpression,
                                     int32_t index ) const
{

  int32_t i = limit( 0, index, ( int32_t )_text.length() );
  int32_t r = regularExpression->search( _text, i, -i );
  if ( r >= 0 )
  {

    return regularExpression->getBeginningOfMatch();

  }
  else
  {

    return r;

  }

}


int32_t gkg::TextBuffer::forwardSearch( const std::string& pattern, 
                                        int32_t index )
{

  if ( _currentRegularExpression )
  {

    delete _currentRegularExpression;

  }
  _currentRegularExpression = new gkg::RegularExpression( pattern );
  return forwardSearch( _currentRegularExpression, index );

}


int32_t gkg::TextBuffer::backwardSearch( const std::string& pattern, 
                                         int32_t index )
{

  if ( _currentRegularExpression )
  {

    delete _currentRegularExpression;

  }
  _currentRegularExpression = new gkg::RegularExpression( pattern );
  return backwardSearch( _currentRegularExpression, index );

}


int32_t gkg::TextBuffer::getSearchBeginning()
{

  return _currentRegularExpression->getBeginningOfMatch();

}


int32_t gkg::TextBuffer::getSearchEnd()
{

  return _currentRegularExpression->getEndOfMatch();

}


int32_t gkg::TextBuffer::match( gkg::RegularExpression* regularExpression,
                                int32_t index, int32_t stop ) const
{

  int32_t s = limit( 0, stop, ( int32_t )_text.length() );
  int32_t i = limit( 0, index, s );
  return regularExpression->match( _text, i );

}


bool gkg::TextBuffer::forwardMatch( gkg::RegularExpression* regularExpression,
                                    int32_t index ) const
{

  int32_t i = limit( 0, index, ( int32_t )_text.length() );
  return regularExpression->match( _text, i ) >= 0;

}


bool gkg::TextBuffer::backwardMatch( gkg::RegularExpression* regularExpression,
                                     int32_t index ) const
{

  int32_t i = limit( 0, index, ( int32_t )_text.length() );
  for ( int32_t j = i; j >= 0; j-- )
  {

    if ( regularExpression->match( _text, j ) == ( i - j ) )
    {

      return true;

    }

  }
  return false;

}


int32_t gkg::TextBuffer::insert( int32_t index, const std::string& text )
{

  if ( !_modified )
  {

    _modified = true;

  }
  if ( ( index < 0 ) || ( index > ( int32_t )_text.length() ) )
  {

    return 0;

  }
  else
  {

    _text.insert( index, text );
    int32_t newLines = ( text.length() == 1 ) ?
                                ( text[ 0 ] == NEWLINE ) :
                                getLinesBetween( index, index + text.length() );
    _lineCount += newLines;
    if ( _lastIndex > index )
    {

      _lastIndex += text.length();
      _lastLine += newLines;

    }
    return text.length();

  }

}


int32_t gkg::TextBuffer::erase( int32_t index, int32_t length )
{

  if ( !_modified )
  {

    _modified = true;

  }
  if ( ( index < 0 ) || ( index > ( int32_t )_text.length() ) )
  {

    return 0;

  }
  else
  {

    length = std::min( length, ( int32_t )_text.length() - index );
    int32_t oldLines = ( length == 1 ) ? ( _text[ index ] == NEWLINE ) :
                                       getLinesBetween( index, index + length );
    if ( _lastIndex > index + length )
    {

      _lastIndex -= length;
      _lastLine -= oldLines;

    }
    else if ( _lastIndex >= index )
    {

      getLineNumber( index );

    }
    _text.erase( index, length );
    _lineCount -= oldLines;
    return length;

  }

}


int32_t gkg::TextBuffer::copy( int32_t index, std::string& buffer, 
                               int32_t length ) const
{

  if ( ( index < 0 ) || ( index > ( int32_t )_text.length() ) )
  {

    return 0;

  }
  else
  {

    length = std::min( length, ( int32_t )_text.length() - index );
    buffer = _text.substr( index, length );
    return length;

  }

}


int32_t gkg::TextBuffer::getHeight () const
{

  return _lineCount;

}


int32_t gkg::TextBuffer::getWidth () const
{

  int32_t width = 0;
  int32_t i = 0;
  while ( i != ( int32_t )_text.length() )
  {

    width = std::max( width, getEndOfLine( i ) - i );
    i = getBeginningOfNextLine( i );

  }
  return width;

}


int32_t gkg::TextBuffer::getLength() const
{

  return ( int32_t )_text.length();

}


const std::string& gkg::TextBuffer::getText() const
{

  return _text;

}


std::string gkg::TextBuffer::getText( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  return _text.substr( index, _text.length() - index );

}


std::string gkg::TextBuffer::getText( int32_t index1, int32_t /*index2*/ ) const
{

  index1 = limit( 0, index1, ( int32_t )_text.length() );
  return _text.substr( index1, _text.length() - index1 );

}


char gkg::TextBuffer::getChar( int32_t index ) const
{

  return ( index < 0 ) ? _text[ 0 ] :
                     ( index > ( int32_t )_text.length() ) ?
                                                      _text[ _text.length() ] :
                                                      _text[ index ];

}


int32_t gkg::TextBuffer::getLineIndex( int32_t line )
{

  int32_t l = ( line < 0 ) ? 0 : ( line >= _lineCount ) ? _lineCount - 1 : line;
  while ( _lastLine > l )
  {

    -- _lastLine;
    _lastIndex = getBeginningOfLine( getEndOfPreviousLine( _lastIndex ) );

  }
  while ( _lastLine < l )
  {

    ++ _lastLine;
    _lastIndex = getBeginningOfNextLine( _lastIndex );

  }
  if ( line >= _lineCount )
  {

    return getEndOfText();

  }
  else
  {

    return _lastIndex;

  }

}


int32_t gkg::TextBuffer::getLinesBetween ( int32_t index1, 
                                           int32_t index2 ) const
{

  if ( index1 == index2 )
  {

    return 0;

  }
  else if ( index1 > index2 )
  {

    return -getLinesBetween( index2, index1 );

  }
  else
  {

    int32_t start =  limit( 0, index1, ( int32_t )_text.length() );
    int32_t finish =  limit( 0, index2, ( int32_t )_text.length() );

    int32_t lineCount = 0;
    while ( start < finish )
    {

      size_t pos = _text.substr( start, finish - start ).find( NEWLINE, 0 );
      if ( pos == std::string::npos )
      {

        break;

      }
      pos += start;
      start = ( int32_t )pos + 1;
      ++ lineCount;

    }
    return lineCount;

  }

}


int32_t gkg::TextBuffer::getLineNumber( int32_t index )
{

  int32_t lineCount = getLinesBetween( _lastIndex, index );
  _lastLine += lineCount;
  _lastIndex = getBeginningOfLine( index );
  return _lastLine;

}


int32_t gkg::TextBuffer::getLineOffset( int32_t index ) const
{

  return ( index < 0 ) ? 0 :
                         ( index > ( int32_t )_text.length() ) ? 0 :
                                            index - getBeginningOfLine( index );

}


int32_t gkg::TextBuffer::getPreviousCharacter( int32_t index ) const
{

  return ( index <= 0 ) ? 0 : index - 1;

}


int32_t gkg::TextBuffer::getNextCharacter( int32_t index ) const
{

  return ( index >= ( int32_t )_text.length() ) ? ( int32_t )_text.length() : 
                                                  index + 1;

}


bool gkg::TextBuffer::isBeginningOfText( int32_t index ) const 
{

  return index <= 0;

}


int32_t gkg::TextBuffer::getBeginningOfText() const
{

  return 0;

}


bool gkg::TextBuffer::isEndOfText( int32_t index ) const
{

  return index >= ( int32_t )_text.length();

}


int32_t gkg::TextBuffer::getEndOfText () const
{

  return ( int32_t )_text.length();

}


bool gkg::TextBuffer::isBeginningOfLine( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  return ( index <= 0 ) || ( _text[ index -1 ] == NEWLINE );

}

int32_t gkg::TextBuffer::getBeginningOfLine( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  while ( ( index > 0 ) && ( _text[ index - 1 ] != NEWLINE ) )
  {

    -- index;

  }
  return index;

}


int32_t gkg::TextBuffer::getBeginningOfNextLine( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  size_t pos = _text.find( NEWLINE, index );
  if ( pos == std::string::npos )
  {

    return _text.length();

  }
  else
  {

    return ( int32_t )pos + 1;

  }

}


bool gkg::TextBuffer::isEndOfLine( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  return ( index >= ( int32_t )_text.length() ) || 
         ( _text[ index ] == NEWLINE );

}


int32_t gkg::TextBuffer::getEndOfLine( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  if ( index == ( int32_t )_text.length() )
  {

    return _text.length();

  }
  size_t pos = _text.find( NEWLINE, index );
  if ( pos == std::string::npos )
  {

    return _text.length();

  }
  else
  {

    return ( int32_t )pos;

  }

}


int32_t gkg::TextBuffer::getEndOfPreviousLine( int32_t index ) const
{

  index = limit( 0, index - 1, ( int32_t )_text.length() );
  while ( ( index > 0 ) && ( _text[ index ] != NEWLINE ) )
  {

    -- index;

  }
  return index;

}


bool gkg::TextBuffer::isBeginningOfWord( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  return ( index <= 0 ) || ( !isalnum( _text[ index - 1 ] ) &&
                             isalnum( _text[ index ] ) );

}


int32_t gkg::TextBuffer::getBeginningOfWord( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  while ( ( index > 0 ) && !( !isalnum( _text[ index - 1 ] ) &&
                               isalnum( _text[ index ] ) ) )
  {

    -- index;

  }
  return index;

}


int32_t gkg::TextBuffer::getBeginningOfNextWord( int32_t index ) const
{

  index = limit( 0, index + 1, ( int32_t )_text.length() );
  while ( ( index < ( int32_t )_text.length() ) &&
          !( !isalnum( _text[ index - 1 ] ) && isalnum( _text[ index ] ) ) )
  {

    ++ index;

  }
  return index;

}


bool gkg::TextBuffer::isEndOfWord( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  return ( index >= ( int32_t )_text.length() ) ||
         ( isalnum( _text[ index - 1 ] ) && !isalnum( _text[ index ] ) );

}


int32_t gkg::TextBuffer::getEndOfWord( int32_t index ) const
{

  index = limit( 0, index, ( int32_t )_text.length() );
  while ( ( index < ( int32_t )_text.length() ) &&
          !( isalnum( _text[ index - 1 ] ) && !isalnum( _text[ index ] ) ) )
  {

    ++ index;

  }
  return index;

}


int32_t gkg::TextBuffer::getEndOfPreviousWord( int32_t index ) const
{

  index = limit( 0, index - 1, ( int32_t )_text.length() );
  while ( ( index > 0 ) && !( isalnum( _text[ index - 1 ] ) &&
                           !isalnum( _text[ index ] ) ) )
  {

    -- index;

  }
  return index;

}


bool gkg::TextBuffer::isBufferModified() const
{

  return _modified;

}


void gkg::TextBuffer::rightTrim()
{

  if ( !_modified )
  {

    _modified = true;

  }
  const std::string& text = getText();
  int32_t byteCount = ( int32_t )text.length();

  while ( isspace( text[ byteCount - 1 ] ) && byteCount )
  {

    erase( 1, byteCount - 1 );
    -- byteCount;

  }

}
