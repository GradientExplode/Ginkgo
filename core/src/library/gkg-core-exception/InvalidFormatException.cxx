#include <gkg-core-exception/InvalidFormatException.h>


gkg::InvalidFormatException::InvalidFormatException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                            : gkg::FormatMismatchException( argument, fileName )
{
}


gkg::InvalidFormatException::InvalidFormatException(
                                                   const std::string& fileName )
                            : gkg::FormatMismatchException(
                                      "invalid format (not handled)", fileName )
{
}


gkg::InvalidFormatException::~InvalidFormatException() throw()
{
}


