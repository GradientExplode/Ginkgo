#include <gkg-core-exception/InvalidPositiveIntegerException.h>


gkg::InvalidPositiveIntegerException::InvalidPositiveIntegerException(
                                                     const std::string& value,
                                                     const std::string& name )
                            : gkg::InvalidIntegerException( value, name )
{
}


gkg::InvalidPositiveIntegerException::~InvalidPositiveIntegerException() throw()
{
}


