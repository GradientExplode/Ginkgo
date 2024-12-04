#ifndef _gkg_graphics_X11_X11_X11FontImplementation_h_
#define _gkg_graphics_X11_X11_X11FontImplementation_h_


#include <gkg-graphics-X11-core/FontImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11FontImplementation : public FontImplementation
{

  public:

    X11FontImplementation( Display* display,
                           const std::string& name,
                           float scale );
    virtual ~X11FontImplementation();

    void getFontBoundingBox( FontBoundingBox& bbox ) const;
    void getCharBoundingBox( int32_t c, FontBoundingBox& bbox ) const;
    void getStringBoundingBox( const std::string& s,
                               FontBoundingBox& bbox ) const;

    FloatCoordinate getWidth( int32_t c ) const;
    FloatCoordinate getWidth( const std::string& s ) const;

    int32_t getIndex( const std::string& s, float offset, bool between ) const;

    XFontStruct* getXFontStruct() const;

    static X11FontImplementation* getImplementation( Font* font,
                                                     Display* display );

  private:

    XFontStruct* _xFontStruct;

};


}


#endif
