#ifndef _gkg_graphics_X11_core_Brush_h_
#define _gkg_graphics_X11_core_Brush_h_


#include <list>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Display;
class BrushImplementation;


//
// class BrushImplementationLut
//

class BrushImplementationLut
{

  public:

    BrushImplementationLut( const int32_t* pattern, int32_t count,
                            FloatCoordinate floatWidth );
    ~BrushImplementationLut();

    FloatCoordinate getWidth() const;

    BrushImplementation* getImplementation( Display* display );

  private:

    int32_t* _pattern;
    int32_t _count;
    FloatCoordinate _floatWidth;
    std::list< BrushImplementation* > _brushImplementations;

};


//
// class Brush
//

class Brush : public RCGUIObject
{

  public:

    Brush( FloatCoordinate floatWidth );
    Brush( const int32_t* pattern, int32_t count, FloatCoordinate floatWidth );
    Brush( int32_t patternVector, FloatCoordinate floatWidth );
    virtual ~Brush();

    virtual FloatCoordinate getWidth() const;

    BrushImplementation* getImplementation( Display* display ) const;

  private:

    BrushImplementationLut* _brushImplementationLut;

};


}


#endif
