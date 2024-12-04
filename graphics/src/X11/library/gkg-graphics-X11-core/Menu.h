#ifndef _gkg_graphics_X11_core_Menu_h_
#define _gkg_graphics_X11_core_Menu_h_


#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-core-pattern/Observable.h>


namespace gkg
{


class Action;
class Menu;
class MenuImplementation;
class Patch;
class TellTaleState;
class Window;


class MenuItem : public Observer
{

  public:

    MenuItem( Glyph* glyph, TellTaleState* tellTaleState );
    MenuItem( Glyph* glyph, TellTaleState* tellTaleState, Action* action );
    MenuItem( Glyph* glyph, TellTaleState* tellTaleState, Menu* menu,
              Window* window = 0 );
    virtual ~MenuItem();

    Glyph* getBody() const;
    TellTaleState* getState() const;

    virtual void setAction( Action* action );
    Action* getAction() const;

    virtual void setMenu( Menu* menu, Window* window = 0 );
    Menu* getMenu() const;

    Window* getWindow() const;
    Patch* getPatch() const;

    virtual void update( const Observable* observable, void* arg = 0 );

  private:

    void initialize( Glyph* glyph, TellTaleState* tellTaleState );

    Patch* _patch;
    TellTaleState* _state;
    Action* _action;
    Menu* _menu;
    Window* _window;

};


// visible list of actions
class Menu : public InputHandler
{

  public:

    Menu( Glyph* glyph,
          Style* style,
          float x1, float y1,
          float x2, float y2 );
    virtual ~Menu();

    virtual void appendItem( MenuItem* menuItem );
    virtual void prependItem( MenuItem* menuItem );
    virtual void insertItem( Glyph::Index index, MenuItem* menuItem );
    virtual void removeItem( Glyph::Index index );
    virtual void replaceItem( Glyph::Index index, MenuItem* menuItem );

    virtual Glyph::Index getItemCount() const;
    virtual MenuItem* getItem( Glyph::Index index ) const;

    virtual void select( Glyph::Index index );
    virtual Glyph::Index getSelected() const;
    virtual void unselect();

    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void release( const Event& event );

    virtual void open();
    virtual void close();

    MenuImplementation* getImplementation() const;

  private:

    MenuImplementation* _menuImplementation;

};


}


#endif
