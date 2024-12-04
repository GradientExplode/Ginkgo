#ifndef _gkg_graphics_X11_core_Visual_h_
#define _gkg_graphics_X11_core_Visual_h_


#include <gkg-graphics-X11-core/Color.h>


namespace gkg
{


class Visual
{

  public:

    virtual~ Visual();

    virtual bool findColor( const std::string& name, 
                            FloatColorIntensity& r,
                            FloatColorIntensity& g,
                            FloatColorIntensity& b ) const = 0;

  protected:

    Visual();

};


}


#endif
