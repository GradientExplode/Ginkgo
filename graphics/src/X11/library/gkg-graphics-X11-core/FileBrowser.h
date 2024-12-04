#ifndef _gkg_graphics_X11_core_FileBrowser_h_
#define _gkg_graphics_X11_core_FileBrowser_h_


#include <gkg-graphics-X11-core/Browser.h>


namespace gkg
{


class Adjustable;
class FileBrowserImplementation;


// browse a directory
class FileBrowser : public Browser
{

  public:

    FileBrowser( Action* accept, Action* cancel );
    virtual ~FileBrowser();

    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void release( const Event& event );
    virtual void keyStroke( const Event& event );

    virtual InputHandler* focusIn();
    virtual void focusOut();

    virtual void select( Glyph::Index index );

    virtual Adjustable* getAdjustable() const;
    virtual void refresh();

    FileBrowserImplementation* getImplementation() const;

  private:

    FileBrowserImplementation* _fileBrowserImplementation;

};


}


#endif
