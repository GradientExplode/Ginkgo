#ifndef _gkg_graphics_X11_core_Dialog_h_
#define _gkg_graphics_X11_core_Dialog_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/InputHandler.h>


namespace gkg
{


class Window;
class Style;


// dialog box
class Dialog : public InputHandler
{

  public:

    Dialog( Glyph* glyph, Style* style );
    virtual ~Dialog();

    bool postFor( Window* window );
    virtual bool postForAligned( Window* window, float xAlign, float yAlign );

    bool postAt( FloatCoordinate x, FloatCoordinate y );
    virtual bool postAtAligned( FloatCoordinate x, FloatCoordinate y,
                                float xAlign, float yAlign );
    virtual bool run();
    virtual void dismiss( bool accept );

  private:

    bool _done;
    bool _accepted;

};


}


#endif
