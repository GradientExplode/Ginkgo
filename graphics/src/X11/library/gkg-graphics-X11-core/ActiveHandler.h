#ifndef _gkg_graphics_X11_core_ActiveHandler_h_
#define _gkg_graphics_X11_core_ActiveHandler_h_


#include <gkg-graphics-X11-core/InputHandler.h>


namespace gkg
{


class ActiveHandler : public InputHandler
{

  public:

    virtual ~ActiveHandler();

    virtual void undraw();

    virtual void move( const Event& event );
    virtual void drag( const Event& event );
    virtual void enter();
    virtual void leave();

  protected:

    ActiveHandler( Glyph* glyph, Style* style );

  private:

    bool _inside;

};


}


#endif
