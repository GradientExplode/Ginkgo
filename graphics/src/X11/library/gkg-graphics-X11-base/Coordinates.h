#ifndef _gkg_graphics_X11_base_Coordinates_h_
#define _gkg_graphics_X11_base_Coordinates_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


typedef float FloatCoordinate;
typedef int32_t IntegerCoordinate;
typedef int32_t PixelCoordinate;


typedef uint32_t DimensionName;


enum DimensionType
{

  DimensionX = 0,
  DimensionY = 1,
  Dimension2d = 2,
  DimensionZ = 2,
  Dimension3d = 3

};


#define fil 10e6


}


#endif
