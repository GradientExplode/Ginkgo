#include <gkg-core-exception/FormatMismatchException.h>


gkg::FormatMismatchException::FormatMismatchException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                             : gkg::FormatException( argument, fileName )
{
}


gkg::FormatMismatchException::FormatMismatchException(
                                                   const std::string& fileName )
                             : gkg::FormatException( "format mismatch",
                                                     fileName )
{
}


gkg::FormatMismatchException::~FormatMismatchException() throw()
{
}


