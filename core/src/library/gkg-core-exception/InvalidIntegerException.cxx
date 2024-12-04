#include <gkg-core-exception/InvalidIntegerException.h>


gkg::InvalidIntegerException::InvalidIntegerException( const std::string& value,
                                                       const std::string& name )
                            : gkg::InvalidNumberException( value, name )
{
}


gkg::InvalidIntegerException::~InvalidIntegerException() throw()
{
}


