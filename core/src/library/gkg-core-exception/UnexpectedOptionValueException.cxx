#include <gkg-core-exception/UnexpectedOptionValueException.h>


gkg::UnexpectedOptionValueException::UnexpectedOptionValueException(
                                                 const std::string& optionName,
                                                 const std::string& value )
                            : std::runtime_error(
                              std::string( "unexpected value for option \"" ) +
                              optionName + "\" (" + value + ")" )
{
}


gkg::UnexpectedOptionValueException::~UnexpectedOptionValueException() throw ()
{
}
