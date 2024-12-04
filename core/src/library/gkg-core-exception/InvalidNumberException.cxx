#include <gkg-core-exception/InvalidNumberException.h>


gkg::InvalidNumberException::InvalidNumberException( const std::string& value,
                                                     const std::string& name )
                            : gkg::FormatException(
                                           value + " is no a valid " + name,
                                           "" )
{
}


gkg::InvalidNumberException::~InvalidNumberException() throw()
{
}


