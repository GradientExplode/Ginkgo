#ifndef _gkg_graphics_X11_core_PredefinedCursor_h_
#define _gkg_graphics_X11_core_PredefinedCursor_h_


#include <gkg-graphics-X11-core/Cursor.h>


namespace gkg
{


class PredefinedCursorImplementation;


class PredefinedCursor : public Cursor
{

  public:

    PredefinedCursor( int32_t code,
                      const Color* foreground = 0,
                      const Color* background = 0 );
    virtual ~PredefinedCursor();

    virtual void makeCursor( Display* display, Visual* visual );

    PredefinedCursorImplementation* getImplementation() const;

  private:

    PredefinedCursorImplementation* _predefinedCursorImplementation;

};


}


#endif
