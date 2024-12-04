#include <gkg-core-exception/MissingOptionException.h>


gkg::MissingOptionException::MissingOptionException(
                                                 const std::string& optionName )
                            : std::runtime_error(
                                std::string( "missing value for option \"") +
                                optionName + "\"" )
{
}


gkg::MissingOptionException::~MissingOptionException() throw ()
{
}
