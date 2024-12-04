#include <gkg-core-exception/ItemTypeException.h>


gkg::ItemTypeException::ItemTypeException( const std::string& argument,
                                           const std::string& fileName )
                       : gkg::IOException( argument, fileName )
{
}


gkg::ItemTypeException::ItemTypeException( const std::string& fileName )
                       : gkg::IOException( "item type error", fileName )
{
}


gkg::ItemTypeException::~ItemTypeException() throw()
{
}


