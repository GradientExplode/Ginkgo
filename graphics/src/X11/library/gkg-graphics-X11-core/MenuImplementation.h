#ifndef _gkg_graphics_X11_core_MenuImplementation_h_
#define _gkg_graphics_X11_core_MenuImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Menu;
class MenuItem;
class Cursor;
class Event;


class MenuImplementation
{

  public:

    MenuImplementation( Menu* menu, float x1, float y1, float x2, float y2 );
    ~MenuImplementation();

    Menu* getTarget() const;

    void appendItem( MenuItem* menuItem );
    void prependItem( MenuItem* menuItem );
    void insertItem( Glyph::Index index, MenuItem* menuItem );
    void removeItem( Glyph::Index index );
    bool replaceItem( Glyph::Index index, MenuItem* menuItem );

    Glyph::Index getItemCount() const;
    MenuItem* getItem( Glyph::Index index ) const;

    void select( Glyph::Index index );
    Glyph::Index getSelected() const;
    void unselect();

    void open();
    void close();

    Menu* getSelectedMenu() const;
    void saveCursor( Canvas* canvas );
    void restoreCursor( Canvas* canvas );
    void grab( Menu* menu, const Event& event );
    void ungrab( Menu* menu, const Event& event );

  private:

    Menu* _target;

    std::deque< MenuItem* > _items;
    Glyph::Index _item;
    float _x1, _y1, _x2, _y2;
    bool _savedCursor;
    Cursor* _cursor;
    bool _grabbed;

};


}


#endif
