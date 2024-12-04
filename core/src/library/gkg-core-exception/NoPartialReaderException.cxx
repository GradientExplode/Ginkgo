#include <gkg-core-exception/NoPartialReaderException.h>


gkg::NoPartialReaderException::NoPartialReaderException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                              : gkg::IOException(
                                           std::string( "no " ) +
                                           "'" + argument + "' partial reader" ,
                                           fileName )
{
}


gkg::NoPartialReaderException::NoPartialReaderException(
                                                   const std::string& fileName )
                              : gkg::IOException( "no partial reader",
                                                  fileName )
{
}


gkg::NoPartialReaderException::~NoPartialReaderException() throw()
{
}


