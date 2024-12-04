#include <gkg-core-exception/DiskFullException.h>


gkg::DiskFullException::DiskFullException( const std::string& argument,
                                           const std::string& fileName )
                       : gkg::ReadWriteException( argument, fileName )
{
}


gkg::DiskFullException::DiskFullException( const std::string& fileName )
                       : gkg::ReadWriteException( "disk full", fileName )
{
}


gkg::DiskFullException::~DiskFullException() throw()
{
}


