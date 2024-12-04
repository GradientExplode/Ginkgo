#include <gkg-core-exception/EofException.h>


gkg::EofException::EofException( const std::string& argument,
                                 const std::string& fileName )
                  : gkg::StreamStateException( argument, fileName )
{
}


gkg::EofException::EofException( const std::string& fileName )
                  : gkg::StreamStateException( "premature end of file",
                                               fileName )
{
}


gkg::EofException::~EofException() throw()
{
}


