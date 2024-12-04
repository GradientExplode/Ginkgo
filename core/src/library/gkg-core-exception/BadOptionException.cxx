#include <gkg-core-exception/BadOptionException.h>


gkg::BadOptionException::BadOptionException( const std::string& name )
                        : std::logic_error( std::string( "\"" ) + name +
                                            "\" is not a valid option name" )
{
}


gkg::BadOptionException::~BadOptionException() throw ()
{
}
