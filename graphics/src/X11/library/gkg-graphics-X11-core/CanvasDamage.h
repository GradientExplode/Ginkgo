#ifndef _gkg_graphics_X11_core_CanvasDamage_h_
#define _gkg_graphics_X11_core_CanvasDamage_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


struct CanvasDamage
{

  CanvasDamage() : left( 0 ), bottom( 0 ), right( 0 ), top( 0 ) {}

  FloatCoordinate left;
  FloatCoordinate bottom;
  FloatCoordinate right;
  FloatCoordinate top;

};


}


#endif
