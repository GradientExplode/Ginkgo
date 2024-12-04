#include <gkg-core-exception/HelpOptionException.h>


gkg::HelpOptionException::HelpOptionException()
                            : std::runtime_error( std::string( "help option") )
{
}


gkg::HelpOptionException::~HelpOptionException() throw ()
{
}
