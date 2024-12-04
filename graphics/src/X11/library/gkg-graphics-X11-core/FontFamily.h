#ifndef _gkg_graphics_X11_core_FontFamily_h_
#define _gkg_graphics_X11_core_FontFamily_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <map>


namespace gkg
{


class Display;
class FontFamilyImplementation;


class FontFamily
{

  public:

    FontFamily( const std::string& familyName );
    virtual ~FontFamily();

    virtual bool getFont( int32_t size, const std::string& style,
                          std::string& name, float& scale );
    virtual bool getFont( int32_t size, std::string& name, float& scale );

    FontFamilyImplementation* getImplementation( Display* display );

  private:

    std::string _familyName;
    std::map< Display*, FontFamilyImplementation* > _fontFamilyImplementations;

};


}


#endif
