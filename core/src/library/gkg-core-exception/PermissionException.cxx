#include <gkg-core-exception/PermissionException.h>


gkg::PermissionException::PermissionException( const std::string& argument,
                                               const std::string& fileName )
                         : gkg::OpenException( argument, fileName )
{
}


gkg::PermissionException::PermissionException( const std::string& fileName )
                         : gkg::OpenException( "no read/write permission",
                                               fileName )
{
}


gkg::PermissionException::~PermissionException() throw()
{
}


