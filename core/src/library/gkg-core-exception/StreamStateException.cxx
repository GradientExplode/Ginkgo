#include <gkg-core-exception/StreamStateException.h>


gkg::StreamStateException::StreamStateException( const std::string& argument,
                                                 const std::string& fileName )
                          : gkg::ReadWriteException( argument, fileName )
{
}


gkg::StreamStateException::~StreamStateException() throw()
{
}


