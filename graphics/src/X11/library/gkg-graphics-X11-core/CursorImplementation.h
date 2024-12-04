#ifndef _gkg_graphics_X11_core_CursorImplementation_h_
#define _gkg_graphics_X11_core_CursorImplementation_h_


#include <string>


namespace gkg
{


class Cursor;
class Color;
class Display;
class Style;


class CursorImplementation
{

  public:

    virtual ~CursorImplementation();

    virtual Cursor* getTarget() const;

    virtual const Color* makeColor( Display* display, Style* style,
                                    const std::string& str1,
                                    const std::string& str2,
                                    const std::string& str3,
                                    const std::string& defaultValue );

    virtual const Color* getForeground() const;
    virtual const Color* getBackground() const;

  protected:

    CursorImplementation( Cursor* cursor,
                          const Color* foreground,
                          const Color* background );

    Cursor* _target;

    const Color* _foreground;
    const Color* _background;
    Display* _display;

};


}


#endif
