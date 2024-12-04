#ifndef _gkg_graphics_X11_X11_X11SelectionManagerImplementation_h_
#define _gkg_graphics_X11_X11_X11SelectionManagerImplementation_h_


#include <gkg-graphics-X11-core/SelectionManagerImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11SelectionManagerImplementation : public SelectionManagerImplementation
{

  public:

    X11SelectionManagerImplementation( SelectionManager* selectionManager,
                                       Display* display,
                                       const std::string& name );
    virtual ~X11SelectionManagerImplementation();

    virtual void own();
    virtual void putValue( const void* data, int32_t length, int32_t format );

    void request( SelectionManager* selectionManager,
                  const XSelectionRequestEvent& xSelectionRequestEvent );
    void notify( SelectionManager* selectionManager,
                 const XSelectionEvent& xSelectionEvent );

    static X11SelectionManagerImplementation* 
      getImplementation( SelectionManager* selectionManager );

  protected:

    SelectionManager* _target;

    XDisplay* _xDisplay;
    XSelectionRequestEvent _xRequest;

};


}


#endif
