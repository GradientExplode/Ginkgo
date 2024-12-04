#ifndef _gkg_graphics_X11_core_PSFont_h_
#define _gkg_graphics_X11_core_PSFont_h_


#include <gkg-graphics-X11-core/Font.h>


namespace gkg
{


class PSFontImplementation;


// use PostScript font metrics
class PSFont : public Font
{

  public:

    PSFont( const std::string& psName,
            FloatCoordinate size,
            const std::string& name,
            float scale );
    virtual ~PSFont();

    virtual const std::string& getName() const;
    virtual const std::string& getEncoding() const;
    virtual FloatCoordinate getSize() const;

    virtual FloatCoordinate getWidth( int32_t c ) const;
    virtual FloatCoordinate getWidth( const std::string& s ) const;

    PSFontImplementation* getImplementation() const;

    static bool exists( const std::string& psName );

  private:

    PSFontImplementation* _psFontImplementation;

};


}


#endif
