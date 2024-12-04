#include <gkg-core-exception/ObjectTypeException.h>


gkg::ObjectTypeException::ObjectTypeException( const std::string& argument,
                                               const std::string& fileName )
                         : gkg::IOException( argument, fileName )
{
}


gkg::ObjectTypeException::ObjectTypeException( const std::string& fileName )
                         : gkg::IOException( "object type error", fileName )
{
}


gkg::ObjectTypeException::~ObjectTypeException() throw()
{
}


