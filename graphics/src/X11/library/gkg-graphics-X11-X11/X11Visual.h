#ifndef _gkg_graphics_X11_X11_X11Visual_h_
#define _gkg_graphics_X11_X11_X11Visual_h_


#include <list>
#include <gkg-graphics-X11-core/Visual.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>
#include <map>


namespace gkg
{


class Display;
class Style;


struct X11OverlayInfo
{

  VisualID id;
  int32_t type;
  int32_t transparent;
  int32_t layer;

};


struct X11VisualInfo
{

  XDisplay* xDisplay;
  int32_t screen;
  int32_t depth;
  XVisual* xVisual;
  XColormap cmap;
  X11OverlayInfo overlay;

};



struct VisualTable
{

  const char* className;
  int32_t classTag;

};


//
// ColorTable
//
typedef std::map< uint32_t, XColor > ColorTable;



//
// RGBTable
//


struct RGB
{


  RGB( uint16_t theRed = 0,
       uint16_t theGreen = 0,
       uint16_t theBlue = 0 ) : red(  theRed ),
                                green(  theGreen ),
                                blue(  theBlue )
  {
  }

  uint16_t red;
  uint16_t green;
  uint16_t blue;

};


struct RGBCompare /*: public std::binary_function< RGB,
                                                 RGB,
                                                 bool >
                    deprecated in c++17 */
{

  typedef RGB first_argument_type;
  typedef RGB second_argument_type;
  typedef bool result_type;

  bool operator()( const RGB& v1, const RGB& v2 ) const
  {

    return ( v1.blue < v2.blue ) ||
           ( ( v1.blue == v2.blue  ) && ( ( v1.green < v2.green ) ||
             ( ( v1.green == v2.green ) && ( v1.red < v2.red ) ) ) );
  }

};

typedef std::map< RGB, XColor, RGBCompare > RGBTable;


//
// X11Visual
//
class X11Visual : public Visual
{

  public:

    X11Visual( const X11VisualInfo& x11VisualInfo );
    ~X11Visual();

    void initColorTables();
    bool findColor( const std::string& name, 
                    FloatColorIntensity& r,
                    FloatColorIntensity& g,
                    FloatColorIntensity& b ) const;
    void findColor( uint32_t pixel, XColor& xColor );
    void findColor( uint16_t r, uint16_t g, uint16_t b,
                    XColor& xColor );

    uint32_t getXor( const Style& style ) const;

    XDisplay* getXDisplay() const;
    int32_t getScreen() const;
    int32_t getDepth() const;
    XVisual* getXVisual() const;
    XColormap getColormap() const;

    const X11VisualInfo& getX11VisualInfo() const;

    static Visual* findVisual( Display* display, Style* style );
    static X11Visual* getImplementation( Visual* visual );

  private:

    void setShift( uint32_t mask, uint32_t& v, uint32_t& shift );

    static void findVisualByClassName( const std::string& name,
                                       X11VisualInfo& x11VisualInfo );
    static bool findLayer( const std::string& name, int32_t& layer );
    static void findOverlay( int32_t layer, X11VisualInfo& x11VisualInfo );
    static void findVisualByInfo( XVisualInfo& xVisualInfo, int32_t mask,
                                  X11VisualInfo& x11VisualInfo );

    static uint32_t MSB( uint32_t i );
    static double distance( uint16_t r,
                            uint16_t g,
                            uint16_t b,
                            const XColor& xColor );
    static uint32_t rescale( uint32_t value,
                             uint32_t inScale,
                             uint32_t outScale );

    X11VisualInfo _x11VisualInfo;
    ColorTable* _colorTable;
    RGBTable* _rgbTable;
    XColor* _localMap;
    uint32_t _localMapSize;
    uint32_t _red;
    uint32_t _redShift;
    uint32_t _green;
    uint32_t _greenShift;
    uint32_t _blue;
    uint32_t _blueShift;
    uint32_t _white;
    uint32_t _xor;

};


}


#endif
