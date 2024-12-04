#ifndef _gkg_graphics_X11_core_Font_h_
#define _gkg_graphics_X11_core_Font_h_


#include <string>
#include <list>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <map>


namespace gkg
{


class Display;
class Font;
class FontImplementation;


//
// class FontImplementationLut
//

struct KnownFonts
{

  std::list< Font* > fonts;
  std::list< FontImplementation* > fontImplementations;

};


class FontImplementationLut
{

  public:

    typedef std::map< std::string,
                      KnownFonts* > NameToKnownFontsHashTable;

    FontImplementationLut( const std::string& name, float scale );
    ~FontImplementationLut();

    void remove( const Font* font );

    FontImplementation* getImplementation( Display* display );
    FontImplementation* getDefaultImplementation();
    void addNewImplementation( KnownFonts* knownFonts,
                               FontImplementation* fontImpl );
    void setEntry( KnownFonts* entry );
    void attach( FontImplementation* fontImpl );

    static const Font* lookup( Display* display, const std::string& name,
                               float scale );
    static FontImplementation* findImplementation(
                    const std::list< FontImplementation* >& fontImplementations,
                    Display* display, float scale );
    static const Font* newFont( const std::string& name,
                                float scale,
                                KnownFonts* knownFonts,
                                FontImplementation* fontImplementation );
    static KnownFonts* known( KnownFonts* knownFonts, const std::string& name );

  private:

    std::string _name;
    float _scale;
    std::list< FontImplementation* > _fontImplementations;
    KnownFonts* _entry;

    static NameToKnownFontsHashTable _fonts;

};


//
// struct FontBoundingBox
//

struct FontBoundingBox
{

  FloatCoordinate leftBearing;
  FloatCoordinate rightBearing;
  FloatCoordinate width;
  FloatCoordinate ascent;
  FloatCoordinate descent;
  FloatCoordinate fontAscent;
  FloatCoordinate fontDescent;

};


//
// class Font
//

class Font : public RCGUIObject
{

  public:

    Font( const std::string& name, float scale = 1.0 );
    virtual ~Font();

    virtual void cleanup();

    virtual const std::string& getName() const;
    virtual const std::string& getEncoding() const;
    virtual FloatCoordinate getSize() const;
    virtual float getScale() const;

    virtual void getFontBoundingBox( FontBoundingBox& bbox ) const;
    virtual void getCharBoundingBox( int32_t c, FontBoundingBox& bbox ) const;
    virtual void getStringBoundingBox( const std::string& s,
                                       FontBoundingBox& bbox ) const;
    virtual FloatCoordinate getWidth( int32_t c ) const;
    virtual FloatCoordinate getWidth( const std::string& s ) const;
    virtual FloatCoordinate getHeight() const;

    virtual int32_t getIndex( const std::string& s, float offset, 
                              bool between ) const;

    FontImplementation* getImplementation( Display* display ) const;

    static const Font* lookup( const std::string& name );
    static bool exists( Display* display, const std::string& name );

  private:

    friend class FontImplementationLut;

    FontImplementationLut* _fontImplementationLut;

};


}


#endif
