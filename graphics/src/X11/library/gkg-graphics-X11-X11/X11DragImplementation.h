#ifndef _gkg_graphics_X11_X11_X11DragImplementation_h_
#define _gkg_graphics_X11_X11_X11DragImplementation_h_


#include <gkg-graphics-X11-core/DragImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>


namespace gkg
{


class Window;


//
// class DragMethod
//

struct DragMethod
{

  virtual~ DragMethod();

  virtual XWindow setup( XDisplay* xDisplay, Event& event, Drag* drag ) = 0;
  virtual bool moveWindow( XDisplay* xDisplay, XWindow xWindow,
                           int32_t x, int32_t y ) = 0;
  virtual void cleanup( XDisplay* xDisplay, XWindow xWindow ) = 0;

};


//
// class DragMethodCursor
//

struct DragMethodCursor : public DragMethod
{

  XWindow setup( XDisplay* xDisplay, Event& event, Drag* drag );
  bool moveWindow( XDisplay* xDisplay, XWindow xWindow, int x, int y );
  void cleanup( XDisplay* xDisplay, XWindow xWindow );

};


//
// class DragMethodWindow
//

class DragMethodWindow : public DragMethod
{

  public:

    XWindow setup( XDisplay* xDisplay, Event& event, Drag* drag );
    bool moveWindow( XDisplay* xDisplay, XWindow xWindow, 
                     int32_t x, int32_t y );
    void cleanup( XDisplay* xDisplay, XWindow xWindow );

  private:

    Window* _dragWindow;
    int32_t _dx;
    int32_t _dy;
    int32_t _wlx;
    int32_t _wly;

};


//
// class X11DragImplementation
//

class X11DragImplementation : public DragImplementation
{

  public:

    X11DragImplementation( Drag* drag );
    ~X11DragImplementation();

    bool event( Event& event );

    static X11DragImplementation*  getImplementation( Drag* drag );

  protected:

    DragMethod* _method;
    DragMethodCursor _methodCursor;
    DragMethodWindow _methodWindow;

};


}


#endif
