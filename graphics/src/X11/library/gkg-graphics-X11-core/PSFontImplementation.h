#ifndef _gkg_graphics_X11_core_PSFontImplementation_h_
#define _gkg_graphics_X11_core_PSFontImplementation_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class PSFont;


class PSFontImplementation
{

  public:

    PSFontImplementation( const std::string& psName,
                          FloatCoordinate size );
    virtual ~PSFontImplementation();

    virtual const std::string& getName() const;
    virtual const std::string& getEncoding() const;
    virtual FloatCoordinate getSize() const;

    virtual FloatCoordinate getWidth( int32_t c ) const;

    static std::string getPostscriptFile( const std::string& psName );

  private:

    std::string _name;
    std::string _encoding;
    FloatCoordinate _size;
    FloatCoordinate _widths[ 256 ];

};


// this function is used to get access to the concrete default Postscript 
// metrics directory and should be implemented in the concrete library
// (in libgkg-gui-X11.so for instance )
std::string getDefaultPSMetricsDirectory();


}


#endif
