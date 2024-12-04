#include <gkg-core-exception/NoWriterException.h>


gkg::NoWriterException::NoWriterException( const std::string& argument,
                                           const std::string& fileName )
                       : gkg::IOException( std::string( "no " ) +
                                           "'" + argument + "' writer" ,
                                           fileName )
{
}


gkg::NoWriterException::NoWriterException( const std::string& fileName )
                       : gkg::IOException( "no writer", fileName )
{
}


gkg::NoWriterException::~NoWriterException() throw()
{
}


