#ifndef _gkg_graphics_X11_X11_X11ManagedWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11ManagedWindowImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/ManagedWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class Style;
class Display;


class X11ManagedWindowImplementation : public ManagedWindowImplementation

{

  public:

    struct ManagedWindowHintInfo
    {

      Style* style;
      PixelCoordinate pixelWidth;
      PixelCoordinate pixelHeight;
      Display* display;
      XWMHints* hints;
      XDisplay* xDisplay;
      XWindow xWindow;

    };
    typedef bool ( X11ManagedWindowImplementation::*HintFunction )(
                                            ManagedWindowHintInfo& );

    X11ManagedWindowImplementation( ManagedWindow* managedWindow );
    ~X11ManagedWindowImplementation();

    void iconify();
    void deiconify();

    void computeGeometry();
    void setProperties();

    void doSetInfoIcon();
    void doSetInfoIconBitmap();
    void doSetInfoIconMask();

    void doSet( HintFunction hintFunction );

    bool setInfoName( ManagedWindowHintInfo& info );
    bool setInfoGeometry( ManagedWindowHintInfo& info );
    bool setInfoGroupLeader( ManagedWindowHintInfo& info );
    bool setInfoTransientFor( ManagedWindowHintInfo& info );
    bool setInfoIcon( ManagedWindowHintInfo& info );
    bool setInfoIconName( ManagedWindowHintInfo& info );
    bool setInfoIconGeometry( ManagedWindowHintInfo& info );
    bool setInfoIconBitmap( ManagedWindowHintInfo& info );
    bool setInfoIconMask( ManagedWindowHintInfo& info );
    bool setInfoAll( ManagedWindowHintInfo& info );

    void setWmNormalHints();
    void setWmName();
    void setWmClass();
    void setWmProtocols();
    void setWmColormapWindows();
    void setWmHints();

    static X11ManagedWindowImplementation* 
      getImplementation( ManagedWindow* managedWindow );

};


}


#endif
