#include <gkg-core-exception/ReadWriteException.h>


gkg::ReadWriteException::ReadWriteException( const std::string& argument,
                                             const std::string& fileName )
                        : gkg::IOException( argument, fileName )
{
}


gkg::ReadWriteException::~ReadWriteException() throw()
{
}


