#ifndef _gkg_graphics_X11_X11_X11ColorImplementation_h_
#define _gkg_graphics_X11_X11_X11ColorImplementation_h_


#include <gkg-graphics-X11-core/ColorImplementation.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11ColorImplementation : public ColorImplementation
{

  public:

    X11ColorImplementation( Visual* visual,
                            FloatColorIntensity r,
                            FloatColorIntensity g,
                            FloatColorIntensity b,
                            float alpha,
                            Color::Operation operation );
    virtual ~X11ColorImplementation();

    const XColor& getXColor() const;
    int32_t getOperation() const;
    Pixmap getStipple() const;

    bool distinguished( const Color* color ) const;
    void getIntensities( FloatColorIntensity& r,
                         FloatColorIntensity& g,
                         FloatColorIntensity& b ) const;

    static X11ColorImplementation* getImplementation( Color* color,
                                                      Visual* visual );

  private:

    Pixmap makeStipple( float alpha );

    static Pixmap _stipples[ 16 ];

    XColor _xColor;
    int32_t _operation;
    Pixmap _stipple;

};


}


#endif
