#include <gkg-core-exception/NoReaderException.h>


gkg::NoReaderException::NoReaderException( const std::string& argument,
                                           const std::string& fileName )
                       : gkg::IOException( std::string( "no " ) +
                                           "'" + argument + "' reader" ,
                                           fileName )
{
}


gkg::NoReaderException::NoReaderException( const std::string& fileName )
                       : gkg::IOException( "no reader", fileName )
{
}


gkg::NoReaderException::~NoReaderException() throw()
{
}


