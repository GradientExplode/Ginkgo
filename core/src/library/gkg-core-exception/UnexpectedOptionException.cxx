#include <gkg-core-exception/UnexpectedOptionException.h>


gkg::UnexpectedOptionException::UnexpectedOptionException(
                                                 const std::string& optionName )
                               : std::runtime_error(
                                 std::string( "unexpected value for option \"") +
                                 optionName + "\"" )
{
}


gkg::UnexpectedOptionException::~UnexpectedOptionException() throw ()
{
}
