#ifndef _gkg_graphics_X11_X11_X11SessionImplementation_h_
#define _gkg_graphics_X11_X11_X11SessionImplementation_h_


#include <gkg-graphics-X11-core/SessionImplementation.h>


namespace gkg
{


class X11SessionImplementation : public SessionImplementation
{

  public:

    X11SessionImplementation( Session* session );
    ~X11SessionImplementation();

    static X11SessionImplementation* getImplementation( Session* session );

  protected:

    void loadGkgDefaults( Style* style, int32_t priority );
    void loadApplicationDefaults( Style* style, int32_t priority );
    void loadDisplayDefaults( Style* style, Display* display, 
                              int32_t priority );
    void loadEnvironment( Style* style, int32_t priority );
    std::string getHome() const;
    std::string getGkgGUILibDir() const;
    std::string getXLibDir() const;

};


}


#endif
