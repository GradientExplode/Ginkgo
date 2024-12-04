#ifndef _gkg_graphics_X11_core_FontFamilyImplementation_h_
#define _gkg_graphics_X11_core_FontFamilyImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class FontFamily;
class Display;


class FontFamilyImplementation
{

  public:

    struct FontNameSet
    {

      int32_t value;
      const char* names[6];

    };

    virtual ~FontFamilyImplementation();

    FontFamily* getTarget() const;
    Display* getDisplay() const;

    int32_t getCount() const;
    int32_t getMinWeight() const;
    int32_t getMaxWeight() const;
    int32_t getMinWidth() const;
    int32_t getMaxWidth() const;
    int32_t getMinSlant() const;
    int32_t getMaxSlant() const;
    int32_t getMinSize() const;
    int32_t getMaxSize() const;

    std::string* getNames() const;
    int32_t* getWeights() const;
    int32_t* getSlants() const;
    int32_t* getWidths() const;
    int32_t* getSizes() const;

    virtual bool getFont( int32_t size, const std::string& style,
                          std::string& name, float& scale );

  protected:

    FontFamilyImplementation( FontFamily* fontFamily,
                              Display* display,
                              const std::string& familyName );

    static int32_t nameToValue( const std::string& name,
                                FontNameSet* values,
                                int32_t def );

    FontFamily* _target;
    Display* _display;
    int32_t _count;
    int32_t _minWeight;
    int32_t _maxWeight;
    int32_t _minWidth;
    int32_t _maxWidth;
    int32_t _minSlant;
    int32_t _maxSlant;
    int32_t _minSize;
    int32_t _maxSize;

    std::string* _names;
    int32_t* _weights;
    int32_t* _slants;
    int32_t* _widths;
    int32_t* _sizes;

    static FontNameSet* _weightNames;
    static FontNameSet* _slantNames;
    static FontNameSet* _widthNames;

};


}


#endif
