#include <gkg-core-exception/StreamFailException.h>


gkg::StreamFailException::StreamFailException( const std::string& argument,
                                               const std::string& fileName )
                         : gkg::StreamStateException( argument, fileName )
{
}


gkg::StreamFailException::StreamFailException( const std::string& fileName )
                         : gkg::StreamStateException( "stream in fail state",
                                                      fileName )
{
}


gkg::StreamFailException::~StreamFailException() throw()
{
}


