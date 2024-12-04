#include <gkg-core-exception/NotOpenException.h>


gkg::NotOpenException::NotOpenException( const std::string& argument,
                                         const std::string& fileName )
                      : gkg::StreamStateException( argument, fileName )
{
}


gkg::NotOpenException::NotOpenException( const std::string& fileName )
                      : gkg::StreamStateException( "stream not open", fileName )
{
}


gkg::NotOpenException::~NotOpenException() throw()
{
}


