#ifndef _gkg_graphics_X11_core_Cursor_h_
#define _gkg_graphics_X11_core_Cursor_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Color;
class Bitmap;
class Font;
class CursorImplementation;
class Display;
class Visual;

static const int32_t cursorHeight = 16;
static const int32_t cursorWidth = 16;

typedef int32_t CursorPattern[ cursorHeight ];


// An input cursor is defined by two 16x16 bitmaps, one that specifies which 
// pixels are to be drawn and one that specifies which pixels are in foreground
// color and which in background color.
// If a device does not support a mask the background pixels are not drawn.

class Cursor
{

  public:


    virtual~ Cursor();

    virtual void makeCursor( Display* display, Visual* visual ) = 0;

    CursorImplementation* getImplementation() const;

    static void initialize();
    static Cursor* getDefault();
    static Cursor* getMenu();
    static Cursor* getArrow();
    static Cursor* getCrossHairs();
    static Cursor* getLeftText();
    static Cursor* getRightText();
    static Cursor* getHourGlass();
    static Cursor* getUpperLeft();
    static Cursor* getUpperRight();
    static Cursor* getLowerLeft();
    static Cursor* getLowerRight();
    static Cursor* getNone();

  protected:

    Cursor( const Color* foreground, const Color* background );

  private:

    static Cursor* _default;
    static Cursor* _menu;
    static Cursor* _arrow;
    static Cursor* _crossHairs;
    static Cursor* _leftText;
    static Cursor* _rightText;
    static Cursor* _hourGlass;
    static Cursor* _upperLeft;
    static Cursor* _upperRight;
    static Cursor* _lowerLeft;
    static Cursor* _lowerRight;
    static Cursor* _none;

    CursorImplementation* _cursorImplementation;

};


}


#endif
