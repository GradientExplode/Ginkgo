#ifndef _gkg_graphics_X11_X11_X11DisplayImplementation_h_
#define _gkg_graphics_X11_X11_X11DisplayImplementation_h_


#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>


#include <map>


namespace gkg
{


class X11DisplayImplementation : public DisplayImplementation
{

  public:

    X11DisplayImplementation( Display* display, const std::string& device );
    ~X11DisplayImplementation();

    void close();

    int32_t getFd() const;

    FloatCoordinate getAWidth() const;
    FloatCoordinate getAHeight() const;
    
    bool getDefaults( std::string& defaults ) const;
    void setStyle( Style* style );

    void setScreen( int32_t screen );

    void flush();
    void sync();

    bool getEvent( Event& event );
    void putEvent( const Event& event );
    bool isClosed();

    void ringBell( int32_t volume );
    void setKeyClick( int32_t percent );
    void setAutoRepeat( bool value );
    void setPointerFeedback( int32_t thresh, int32_t scale );
    void movePointer( FloatCoordinate x, FloatCoordinate y );

    XDisplay* getXDisplay() const;
    XWindow getXRoot() const;
    uint32_t getScreen() const;

    void addWindow( Window* window );
    void removeWindow( Window* window );
    const std::map< XWindow, Window* >& getWindows() const;

    static X11DisplayImplementation* getImplementation( Display* display );

  private:

    static double millimetersToPoints( double millimeters );

    XDisplay* _xDisplay;
    XWindow _xRoot;
    uint32_t _screen;

    std::map< XWindow, Window* > _windows;

};


}


#endif
