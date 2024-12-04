#ifndef _gkg_graphics_X11_core_FontImplementation_h_
#define _gkg_graphics_X11_core_FontImplementation_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Display;
class Font;
struct FontBoundingBox;
struct KnownFonts;

//
// class FontImplementation
//

class FontImplementation : public RCGUIObject
{

  public:

    virtual ~FontImplementation();

    virtual Display* getDisplay() const;

    virtual const std::string& getName() const;
    virtual const std::string& getEncoding() const;
    virtual float getSize() const;
    virtual float getScale() const;
    virtual bool isUnscaled() const;

    virtual void getFontBoundingBox( FontBoundingBox& bbox ) const = 0;
    virtual void getCharBoundingBox( int32_t c,
                                     FontBoundingBox& bbox ) const = 0;
    virtual void getStringBoundingBox( const std::string& s,
                                       FontBoundingBox& bbox ) const = 0;

    virtual FloatCoordinate getWidth( int32_t c ) const = 0;
    virtual FloatCoordinate getWidth( const std::string& s ) const = 0;

    virtual int32_t getIndex( const std::string& s, float offset, 
                              bool between ) const = 0;

    virtual void setEntry( KnownFonts* entry );

  protected:

    FontImplementation( Display* display,
                        const std::string& name,
                        float scale );

    Display* _display;
    KnownFonts* _entry;
    float _scale;
    bool _unscaled;
    std::string _name;
    std::string _encoding;
    float _size;

};


}


#endif
