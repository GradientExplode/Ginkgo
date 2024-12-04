#ifndef _gkg_graphics_X11_core_BitmapCursorImplementation_h_
#define _gkg_graphics_X11_core_BitmapCursorImplementation_h_


namespace gkg
{


class Display;
class Visual;
class BitmapCursor;
class Bitmap;


class BitmapCursorImplementation
{

  public:

    virtual ~BitmapCursorImplementation();

    virtual void makeCursor( Display* display, Visual* visual ) = 0;

  protected:

    BitmapCursorImplementation( BitmapCursor* bitmapCursor,
                                const Bitmap* pattern, const Bitmap* mask );

    BitmapCursor* _target;

    const Bitmap* _pattern;
    const Bitmap* _mask;

};


}


#endif
