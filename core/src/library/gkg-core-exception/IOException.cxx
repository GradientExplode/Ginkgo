#include <gkg-core-exception/IOException.h>
#include <gkg-core-exception/BadMediumException.h>
#include <gkg-core-exception/CorruptStreamException.h>
#include <gkg-core-exception/DataTypeFormatException.h>
#include <gkg-core-exception/DiskFullException.h>
#include <gkg-core-exception/EofException.h>
#include <gkg-core-exception/ErrnoException.h>
#include <gkg-core-exception/FileException.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/FormatException.h>
#include <gkg-core-exception/FormatMismatchException.h>
#include <gkg-core-exception/InvalidBooleanException.h>
#include <gkg-core-exception/InvalidFormatException.h>
#include <gkg-core-exception/InvalidIntegerException.h>
#include <gkg-core-exception/InvalidNumberException.h>
#include <gkg-core-exception/InvalidPositiveIntegerException.h>
#include <gkg-core-exception/IsDirException.h>
#include <gkg-core-exception/NoDirException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-exception/OpenException.h>
#include <gkg-core-exception/OutOfRangeException.h>
#include <gkg-core-exception/ParseException.h>
#include <gkg-core-exception/PermissionException.h>
#include <gkg-core-exception/QuotaExceededException.h>
#include <gkg-core-exception/ReadWriteException.h>
#include <gkg-core-exception/StreamFailException.h>
#include <gkg-core-exception/StreamStateException.h>
#include <gkg-core-exception/SyntaxCheckException.h>
#include <gkg-core-exception/TooManyDescriptorsException.h>
#include <gkg-core-exception/WrongFormatException.h>
#include <cerrno>
#include <cstring>

namespace gkg
{

static void copyException( int32_t p, int32_t n, std::exception& e, 
                           int32_t& priority, int32_t& type, 
			   std::string& message );

}


void gkg::copyException( int32_t p, int32_t n, std::exception& e, 
                         int32_t& priority, int32_t& type, 
			 std::string& message )
{

  if ( priority < p )
  {

    priority = p;
    type = n;
    message = e.what();
  }

}


gkg::IOException::IOException( const std::string& argument,
                               const std::string& fileName )
                 : std::runtime_error( argument ),
                   _fileName( fileName )
{
}


gkg::IOException::~IOException() throw()
{
}


const char* gkg::IOException::what() const throw()
{

  if ( !_fileName.empty() )
  {

    _formedMessage = _fileName + " : " + std::runtime_error::what();
    return _formedMessage.c_str();

  }
  else
  {

    return std::runtime_error::what();

  }

}


const std::string& gkg::IOException::getFileName() const
{

  return _fileName;

}


void gkg::IOException::launchErrno( const std::string& fileName )
{

  switch( errno )
  {

    case ENOENT:

      throw gkg::FileNotFoundException( std::strerror( errno ), fileName );

    case EIO:

      throw gkg::ReadWriteException( std::strerror( errno ), fileName );

    case EACCES:

      throw gkg::PermissionException( std::strerror( errno ), fileName );

    case EISDIR:

      throw gkg::IsDirException( std::strerror( errno ), fileName );

    case EMFILE:

      throw gkg::TooManyDescriptorsException( std::strerror( errno ),fileName );

    case ENOSPC:

      throw gkg::DiskFullException( std::strerror( errno ), fileName );

    case EROFS:

      throw gkg::PermissionException( std::strerror( errno ), fileName );

#ifdef ECOMM

    case ECOMM:

      throw gkg::BadMediumException( std::strerror( errno ), fileName );

#endif

#ifdef EBADFD

    case EBADFD:

      throw gkg::StreamStateException( std::strerror( errno ), fileName );

#endif

#ifdef ENETDOWN

    case ENETDOWN:

#endif

#ifdef ENETUNREACH

    case ENETUNREACH:

#endif

#ifdef ENETRESET

    case ENETRESET:

#endif

#ifdef ENOMEDIUM

    case ENOMEDIUM:

#endif

#ifdef EMEDIUMTYPE

    case EMEDIUMTYPE:

#endif

      throw gkg::BadMediumException( std::strerror( errno ), fileName );

    default:

      throw gkg::IOException( std::strerror( errno ), fileName );

  }

}



void gkg::IOException::keepPriority( std::exception& e,
                                     int32_t& priority, 
                                     int32_t& type,
                                     std::string& message, 
                                     int32_t raisePriority )
{

  {

    gkg::CorruptStreamException* 
      te = dynamic_cast< gkg::CorruptStreamException* >( &e );
    if ( te )
    {

      gkg::copyException( 10 + raisePriority, 1, e, priority, type, message );
      return;

    }

  }

  {

    gkg::InvalidFormatException*
      te = dynamic_cast< gkg::InvalidFormatException* >( &e );
    if ( te )
    {

      gkg::copyException( 9 + raisePriority, 2, e, priority, type, message );
      return;

    }

  }

  {

    gkg::FormatMismatchException*
      te = dynamic_cast< gkg::FormatMismatchException* >( &e );
    if ( te )
    {

      gkg::copyException( 8 + raisePriority, 3, e, priority, type, message );
      return;

    }

  }

  {

    gkg::WrongFormatException*
      te = dynamic_cast< gkg::WrongFormatException* >( &e );
    if ( te )
    {

      gkg::copyException( 3 + raisePriority, 4, e, priority, type, message );
      return;

    }

  }

  {

    gkg::StreamFailException*
      te = dynamic_cast< gkg::StreamFailException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 6, e, priority, type, message );
      return;

    }

  }

  {
    gkg::NotOpenException*
      te = dynamic_cast< gkg::NotOpenException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 7, e, priority, type, message );
      return;

    }

  }

  {

    gkg::EofException*
      te = dynamic_cast< gkg::EofException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 8, e, priority, type, message );
      return;

    }

  }

  {

    gkg::StreamStateException*
      te = dynamic_cast< gkg::StreamStateException* >( &e );
    if ( te )
    {

      gkg::copyException( 4 + raisePriority, 9, e, priority, type, message );
      return;

    }

  }

  {

    gkg::QuotaExceededException*
      te = dynamic_cast< gkg::QuotaExceededException* >( &e );
    if ( te )
    {

      gkg::copyException( 8 + raisePriority, 10, e, priority, type, message );
      return;

    }

  }

  {

    gkg::DiskFullException*
      te = dynamic_cast< gkg::DiskFullException* >( &e );
    if ( te )
    {

      gkg::copyException( 8 + raisePriority, 11, e, priority, type, message );
      return;

    }

  }

  {

    gkg::BadMediumException*
      te = dynamic_cast< gkg::BadMediumException* >( &e );
    if ( te )
    {

      gkg::copyException( 9 + raisePriority, 12, e, priority, type, message );
      return;

    }

  }

  {

    gkg::ReadWriteException*
      te = dynamic_cast< gkg::ReadWriteException* >( &e );
    if ( te )
    {

      gkg::copyException( 6 + raisePriority, 13, e, priority, type, message );
      return;

    }

  }

  {

    gkg::TooManyDescriptorsException*
      te = dynamic_cast< gkg::TooManyDescriptorsException* >( &e );
    if ( te )
    {

      gkg::copyException( 7 + raisePriority, 14, e, priority, type, message );
      return;

    }

  }

  {

    gkg::NoDirException*
      te = dynamic_cast< gkg::NoDirException* >( &e );
    if ( te )
    {

      gkg::copyException( 8 + raisePriority, 15, e, priority, type, message );
      return;

    }

  }

  {

    gkg::PermissionException*
      te = dynamic_cast< gkg::PermissionException* >( &e );
    if ( te )
    {

      gkg::copyException( 8 + raisePriority, 16, e, priority, type, message );
      return;

    }

  }

  {

    gkg::FileNotFoundException*
      te = dynamic_cast< gkg::FileNotFoundException* >( &e );
    if ( te )
    {

      gkg::copyException( 1 + raisePriority, 17, e, priority, type, message );
      return;

    }

  }

  {

    gkg::OpenException*
      te = dynamic_cast< gkg::OpenException* >( &e );
    if ( te )
    {

      gkg::copyException( 1 + raisePriority, 18, e, priority, type, message );
      return;

    }

  }

  {

    gkg::DataTypeFormatException*
      te = dynamic_cast< gkg::DataTypeFormatException* >( &e );
    if ( te )
    {

      gkg::copyException( 9 + raisePriority, 20, e, priority, type, message );
      return;

    }

  }

  {

    gkg::IsDirException*
      te = dynamic_cast< gkg::IsDirException* >( &e );
    if ( te )
    {

      gkg::copyException( 7 + raisePriority, 21, e, priority, type, message );
      return;

    }

  }

  {

    gkg::SyntaxCheckException*
      te = dynamic_cast< gkg::SyntaxCheckException* >( &e );
    if ( te )
    {

      gkg::copyException( 10 + raisePriority, 23, e, priority, type, message );
      return;

    }

  }

  {

    gkg::ErrnoException*
      te = dynamic_cast< gkg::ErrnoException* >( &e );
    if ( te )
    {

      gkg::copyException( 1 + raisePriority, 24, e, priority, type, message );
      return;

    }

  }

  {

    gkg::FileException*
      te = dynamic_cast< gkg::FileException* >( &e );
    if ( te )
    {

      gkg::copyException( 1 + raisePriority, 25, e, priority, type, message );
      return;

    }

  }

  {

    gkg::InvalidPositiveIntegerException*
      te = dynamic_cast< gkg::InvalidPositiveIntegerException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 26, e, priority, type, message );
      return;

    }

  }

  {

    gkg::InvalidIntegerException*
      te = dynamic_cast< gkg::InvalidIntegerException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 27, e, priority, type, message );
      return;

    }

  }

  {

    gkg::InvalidBooleanException*
      te = dynamic_cast< gkg::InvalidBooleanException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 31, e, priority, type, message );
      return;

    }

  }

  {

    gkg::InvalidNumberException*
      te = dynamic_cast< gkg::InvalidNumberException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 28, e, priority, type, message );
      return;

    }

  }

  {

    gkg::OutOfRangeException*
      te = dynamic_cast< gkg::OutOfRangeException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 30, e, priority, type, message );
      return;

    }

  }

  {

    std::range_error* te = dynamic_cast< std::range_error* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 29, e, priority, type, message );
      return;

    }

  }

  {

    gkg::ParseException*
      te = dynamic_cast< gkg::ParseException* >( &e );
    if ( te )
    {

      gkg::copyException( 5 + raisePriority, 22, e, priority, type, message );
      return;

    }

  }

  {

    gkg::FormatException*
      te = dynamic_cast< gkg::FormatException* >( &e );
    if ( te )
    {

      gkg::copyException( 2 + raisePriority, 5, e, priority, type, message );
      return;

    }

  }

  {

    gkg::IOException*
      te = dynamic_cast< gkg::IOException* >( &e );
    if ( te )
    {

      gkg::copyException( 1 + raisePriority, 19, e, priority, type, message );
      return;

    }

  }

  // default non-io exception
  gkg::copyException( raisePriority, -1, e, priority, type, message );

}


void gkg::IOException::launch( int32_t code,
                               const std::string& message, 
                               const std::string& defaultMessage )
{

  switch( code )
  {

    case 1:

      throw gkg::CorruptStreamException( message, "" );

    case 2:

      throw gkg::InvalidFormatException( message, "" );

    case 3:

      throw gkg::FormatMismatchException( message, "" );

    case 4:

      throw gkg::WrongFormatException( message, "" );

    case 5:

      throw gkg::FormatException( message, "" );

    case 6:

      throw gkg::StreamFailException( message, "" );

    case 7:

      throw gkg::NotOpenException( message, "" );

    case 8:

      throw gkg::EofException( message, "" );

    case 9:

      throw gkg::StreamStateException( message, "" );

    case 12:

      throw gkg::BadMediumException( message, "" );

    case 13:

      throw gkg::ReadWriteException( message, "" );

    case 14:

      throw gkg::TooManyDescriptorsException( message, "" );

    case 15:

      throw gkg::NoDirException( message, "" );

    case 16:

      throw gkg::PermissionException( message, "" );

    case 17:

      throw gkg::FileNotFoundException( message, "" );

    case 18:

      throw gkg::OpenException( message, "" );

    case 19:

      throw gkg::IOException( message, "" );

    case 20:

      throw gkg::DataTypeFormatException( message, "" );

    case 21:

      throw gkg::IsDirException( message, "" );

    case 22:

      throw gkg::ParseException( message, "", 0 );

    case 23:

      throw gkg::SyntaxCheckException( message, "" );

    case 24:

      throw gkg::ErrnoException();

    case 25:

      throw gkg::FileException( message, "" );

    case 26:

      throw gkg::InvalidPositiveIntegerException( message );

    case 27:

      throw gkg::InvalidIntegerException( message );

    case 28:

      throw gkg::InvalidNumberException( message );

    case 29:

      throw std::range_error( message );

    case 30:

      throw gkg::OutOfRangeException( message );

    case 31:

      throw gkg::InvalidBooleanException( message );

    default:

      if ( defaultMessage.empty() )
      {

	if ( message.empty() )
        {

	  throw std::runtime_error( "exception (no message)" );

        }
	else
	  throw gkg::WrongFormatException( message, "" );

      }
      throw gkg::WrongFormatException( message + defaultMessage, "" );

  }

}
