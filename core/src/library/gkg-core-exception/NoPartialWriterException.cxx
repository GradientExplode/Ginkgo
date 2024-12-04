#include <gkg-core-exception/NoPartialWriterException.h>


gkg::NoPartialWriterException::NoPartialWriterException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                              : gkg::IOException(
                                           std::string( "no " ) +
                                           "'" + argument + "' partial writer" ,
                                           fileName )
{
}


gkg::NoPartialWriterException::NoPartialWriterException(
                                                   const std::string& fileName )
                              : gkg::IOException( "no partial writer",
                                                  fileName )
{
}


gkg::NoPartialWriterException::~NoPartialWriterException() throw()
{
}


