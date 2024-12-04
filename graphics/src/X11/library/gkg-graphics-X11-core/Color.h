#ifndef _gkg_graphics_X11_core_Color_h_
#define _gkg_graphics_X11_core_Color_h_


#include <list>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-base/Color.h>
#include <map>
#include <string>


namespace gkg
{


class Display;
class Color;
class ColorImplementation;
class Visual;


struct ColorEntry
{

  ColorEntry( Display* theDisplay, std::string theName )
            : display( theDisplay ),
              name( theName )
  {}

  Display* display;
  std::string name;

};


struct ColorEntryCompare /*: public std::binary_function< ColorEntry,
                                                        ColorEntry,
                                                        bool >
                          deprecated in c++17 */
{

  typedef ColorEntry first_argument_type;
  typedef ColorEntry second_argument_type;
  typedef bool result_type;

  bool operator()( const ColorEntry& c1, const ColorEntry& c2 ) const
  {

    return ( ( uint64_t )c1.display < ( uint64_t)c2.display ) ||
           ( ( ( uint64_t )c1.display == ( uint64_t )c2.display  ) &&
	       ( c1.name < c2.name ) );
  }

};


}


namespace gkg
{


class ColorImplementationLut;


class Color : public RCGUIObject
{

  public:

    enum Operation
    {

      Copy,
      Xor,
      Invisible

    };

    Color( FloatColorIntensity r,
           FloatColorIntensity g,
           FloatColorIntensity b,
           float alpha = 1.0f,
           Operation operation = Copy );
    Color( const Color& other,
           float alpha = 1.0f,
           Operation operation = Copy );
    virtual ~Color();

    virtual FloatColorIntensity getRed() const;
    virtual FloatColorIntensity getGreen() const;
    virtual FloatColorIntensity getBlue() const;
    virtual float getAlpha() const;
    virtual Operation getOperation() const;
    virtual const std::string& getName() const;
    virtual Display* getDisplay() const;

    virtual bool distinguished( Display* display, const Color* color ) const;
    virtual void getIntensities( Display* display,
                                 FloatColorIntensity& r,
                                 FloatColorIntensity& g,
                                 FloatColorIntensity& b ) const;

    // use default display
    virtual bool distinguished( const Color* color ) const;
    virtual void getIntensities( FloatColorIntensity& r,
                                 FloatColorIntensity& g,
                                 FloatColorIntensity& b ) const;

    virtual Color* getBrightness( float adjust ) const;

    ColorImplementation* getImplementation( Visual* visual ) const;

    static const Color* lookup( Display* display, const std::string& name );

  private:

    friend class ColorImplementationLut;

    ColorImplementationLut* _colorImplementationLut;

};



class ColorImplementationLut
{

  public:

    typedef std::map< ColorEntry, Color*, ColorEntryCompare >
      NameToColorHashTable;

    ColorImplementationLut( FloatColorIntensity red,
                            FloatColorIntensity green,
                            FloatColorIntensity blue,
                            float alpha,
                            Color::Operation operation );
    ~ColorImplementationLut();

    FloatColorIntensity getRed() const;
    FloatColorIntensity getGreen() const;
    FloatColorIntensity getBlue() const;
    float getAlpha() const;
    Color::Operation getOperation() const;
    const std::string& getName() const;
    Display* getDisplay() const;

    void setName( const std::string& name );
    void setDisplay( Display* display );

    ColorImplementation* getImplementation( Visual* visual );

    static const Color* lookup( Display* display, const std::string& name );

  private:

    FloatColorIntensity _red;
    FloatColorIntensity _green;
    FloatColorIntensity _blue;
    float _alpha;
    Color::Operation _operation;
    std::list< ColorImplementation* > _colorImplementations;
    std::string _name;
    Display* _display;

    static NameToColorHashTable _colors;

};



}


#endif
