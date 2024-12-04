#include <gkg-core-exception/ErrnoException.h>
#include <cstring>
#include <cerrno>
#include <cstring>


gkg::ErrnoException::ErrnoException()
                    : std::runtime_error( initialize() ),
                      _gkgErrno( errno )
{
}


gkg::ErrnoException::ErrnoException( const std::string& what )
                    : std::runtime_error( what ),
                      _gkgErrno( errno )
{
}


gkg::ErrnoException::~ErrnoException() throw ()
{
}


int32_t gkg::ErrnoException::getThrownErrno() const
{

  return _gkgErrno;

}


std::string gkg::ErrnoException::initialize()
{

  return std::string( ::strerror( errno ) );

}
