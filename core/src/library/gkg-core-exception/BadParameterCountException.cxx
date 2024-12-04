#include <gkg-core-exception/BadParameterCountException.h>
#include <gkg-core-io/StringConverter.h>


gkg::BadParameterCountException::BadParameterCountException(
                                                   int32_t count,
                                                   const std::string& argument )
                                : std::runtime_error( argument +
                                       "bad parameter count, " +
                                       gkg::StringConverter::toString( count ) +
                                       " are required" )
{
}


gkg::BadParameterCountException::~BadParameterCountException() throw()
{
}


