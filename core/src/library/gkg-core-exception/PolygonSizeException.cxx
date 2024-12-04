#include <gkg-core-exception/PolygonSizeException.h>


gkg::PolygonSizeException::PolygonSizeException( const std::string& argument,
                                                 const std::string& fileName )
                          : gkg::IOException( argument, fileName )
{
}


gkg::PolygonSizeException::PolygonSizeException( const std::string& fileName )
                          : gkg::IOException( "polygon size error", fileName )
{
}


gkg::PolygonSizeException::~PolygonSizeException() throw()
{
}


