#ifndef _gkg_graphics_X11_X11_X11BrushImplementation_h_
#define _gkg_graphics_X11_X11_X11BrushImplementation_h_


#include <gkg-graphics-X11-core/BrushImplementation.h>



namespace gkg
{


class X11BrushImplementation : public BrushImplementation
{

  public:

    X11BrushImplementation( const int32_t* pattern,
                            int32_t count,
                            PixelCoordinate pixelWidth,
                            Display* display );
    ~X11BrushImplementation();

    char* getDashList() const;
    int32_t getDashCount() const;

    static X11BrushImplementation* getImplementation( Brush* Brush,
                                                      Display* display );

  private:

    char* _dashList;
    int32_t _dashCount;

};


}


#endif
