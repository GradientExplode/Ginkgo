#ifndef _gkg_graphics_X11_X11_X11FontFamilyImplementation_h_
#define _gkg_graphics_X11_X11_X11FontFamilyImplementation_h_


#include <gkg-graphics-X11-core/FontFamilyImplementation.h>


namespace gkg
{


class X11FontFamilyImplementation : public FontFamilyImplementation
{

  public:

    X11FontFamilyImplementation( FontFamily* fontFamily,
                                 Display* display,
                                 const std::string& familyName );
    virtual ~X11FontFamilyImplementation();

    static X11FontFamilyImplementation*
             getImplementation( FontFamily* fontFamily, Display* display );

};


}


#endif
