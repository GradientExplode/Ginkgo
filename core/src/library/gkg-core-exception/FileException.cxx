#include <gkg-core-exception/FileException.h>
#include <cerrno>
#include <cstring>


gkg::FileException::FileException( const std::string& fileName )
                   : gkg::ErrnoException( initialize( fileName ) ),
                     _fileName( fileName )
{
}


gkg::FileException::FileException( const std::string&,
                                   const std::string& fileName )
                   : gkg::ErrnoException( initialize( fileName ) ),
                     _fileName( fileName )
{
}


gkg::FileException::~FileException() throw ()
{
}


const std::string& gkg::FileException::getFileName() const
{

  return _fileName;

}


std::string gkg::FileException::initialize( const std::string& fileName )
{

  std::string s( fileName );
  s += ": ";
  s += std::strerror( errno );
  return s;

}
