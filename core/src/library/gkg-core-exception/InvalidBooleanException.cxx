#include <gkg-core-exception/InvalidBooleanException.h>


gkg::InvalidBooleanException::InvalidBooleanException(
                                                      const std::string& value )
                         : gkg::FormatException( value + 
                                                 " is not a valid boolean" )
{
}


gkg::InvalidBooleanException::~InvalidBooleanException() throw()
{
}


