#ifndef _gkg_graphics_X11_core_FontCursor_h_
#define _gkg_graphics_X11_core_FontCursor_h_


#include <gkg-graphics-X11-core/Cursor.h>


namespace gkg
{


class FontCursorImplementation;


class FontCursor : public Cursor
{

  public:

    FontCursor( const Font* font, int32_t pattern, int32_t mask,
                const Color* foreground = 0,
                const Color* background = 0 );
    virtual ~FontCursor();

    virtual void makeCursor( Display* display, Visual* visual );

    FontCursorImplementation* getImplementation() const;

  private:

    FontCursorImplementation* _fontCursorImplementation;

};


}


#endif
