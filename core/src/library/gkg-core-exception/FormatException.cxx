#include <gkg-core-exception/FormatException.h>


gkg::FormatException::FormatException( const std::string& argument,
                                       const std::string& fileName )
                     : gkg::IOException( argument, fileName )
{
}


gkg::FormatException::FormatException( const std::string& fileName )
                     : gkg::IOException( "format error", fileName )
{
}


gkg::FormatException::~FormatException() throw()
{
}


