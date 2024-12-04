#ifndef _gkg_graphics_X11_core_BitmapCursor_h_
#define _gkg_graphics_X11_core_BitmapCursor_h_


#include <gkg-graphics-X11-core/Cursor.h>


namespace gkg
{


class BitmapCursorImplementation;


class BitmapCursor : public Cursor
{

  public:

    BitmapCursor( const Bitmap* pattern, const Bitmap* mask,
                  const Color* foreground = 0,
                  const Color* background = 0 );
    virtual ~BitmapCursor();

    virtual void makeCursor( Display* display, Visual* visual );

    BitmapCursorImplementation* getImplementation() const;

  private:

    BitmapCursorImplementation* _bitmapCursorImplementation;

};


}


#endif
