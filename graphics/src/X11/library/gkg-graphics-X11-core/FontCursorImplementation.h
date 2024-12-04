#ifndef _gkg_graphics_X11_core_FontCursorImplementation_h_
#define _gkg_graphics_X11_core_FontCursorImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Display;
class Visual;
class FontCursor;
class Font;


class FontCursorImplementation
{

  public:

    virtual ~FontCursorImplementation();

    virtual void makeCursor( Display* display, Visual* visual ) = 0;



  protected:

    FontCursorImplementation( FontCursor* fontCursor,
                              const Font* font, int32_t pattern, int32_t mask );

    FontCursor* _target;

    const Font* _font;
    int32_t _pattern;
    int32_t _mask;

};


}


#endif
