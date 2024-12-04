#include <gkg-graphics-X11-core/MenuImplementation.h>
#include <gkg-graphics-X11-core/Menu.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Window.h>


gkg::MenuImplementation::MenuImplementation( gkg::Menu* menu,
                                                float x1, float y1,
                                                float x2, float y2 )
                        : _target( menu ),
                          _item( -1 ),
                          _x1( x1 ),
                          _y1( y1 ),
                          _x2( x2 ),
                          _y2( y2 ),
                          _savedCursor( false ),
                          _cursor( 0 ),
                          _grabbed( false )
{
}


gkg::MenuImplementation::~MenuImplementation()
{

  std::deque< gkg::MenuItem* >::iterator i = _items.begin(),
                                         ie = _items.end();
  while ( i != ie )
  {

    delete *i;
    ++ i;

  }
  _items.clear();

}


gkg::Menu* gkg::MenuImplementation::getTarget() const
{

  return _target;

}


void gkg::MenuImplementation::appendItem( gkg::MenuItem* menuItem )
{

  _items.push_back( menuItem );

}


void gkg::MenuImplementation::prependItem( gkg::MenuItem* menuItem )
{

  _items.push_front( menuItem );

}


void gkg::MenuImplementation::insertItem( gkg::Glyph::Index index,
                                          gkg::MenuItem* menuItem )
{

  _items.insert( _items.begin() + index, menuItem );

}


void gkg::MenuImplementation::removeItem( gkg::Glyph::Index index )
{

  _items.erase( _items.begin() + index );

}


bool gkg::MenuImplementation::replaceItem( gkg::Glyph::Index index,
                                           gkg::MenuItem* menuItem )
{

  if ( ( index >= 0 ) && ( index < ( gkg::Glyph::Index )_items.size() ) )
  {

    gkg::MenuItem* oldMenuItem = _items[ index ];
    delete oldMenuItem;
    _items[ index ] = menuItem;
    return true;

  }
  return false;

}


gkg::Glyph::Index gkg::MenuImplementation::getItemCount() const
{

  return ( gkg::Glyph::Index )_items.size();

}


gkg::MenuItem* 
gkg::MenuImplementation::getItem( gkg::Glyph::Index index ) const
{

  if ( ( index < 0 ) && ( index >= ( gkg::Glyph::Index )_items.size() ) )
  {

    return 0;

  }
  return _items[ index ];

}


void gkg::MenuImplementation::select( gkg::Glyph::Index index )
{

  if ( ( index != _item ) &&
       ( index >= 0 ) &&
       ( index < ( gkg::Glyph::Index )_items.size() ) )
  {

    _target->close();
    _item = index;
    _target->open();

  }

}


gkg::Glyph::Index gkg::MenuImplementation::getSelected() const
{

  return _item;

}


void gkg::MenuImplementation::unselect()
{

  if ( _item != -1 )
  {

    _target->close();
    _item = -1;

  }

}


void gkg::MenuImplementation::open()
{

  if ( _item >= 0 )
  {

    MenuItem* menuItem = getItem( _item );
    gkg::TellTaleState* tellTaleState = menuItem->getState();
    if ( tellTaleState && tellTaleState->test( gkg::TellTaleState::Enabled ) )
    {

      tellTaleState->set( gkg::TellTaleState::Active, true );
      if ( menuItem->getMenu() )
      {

        menuItem->getMenu()->unselect();
        gkg::Action* action = menuItem->getAction();
        if ( action )
        {

          action->execute();

        }

      }
      if ( menuItem->getWindow() )
      {

        const gkg::Window*
          rel = menuItem->getPatch()->getCanvas()->getWindow();
        const gkg::GeometryAllocation&
          geometryAllocation = menuItem->getPatch()->getGeometryAllocation();
        gkg::Window* window = menuItem->getWindow();
        window->setDisplay( rel->getDisplay() );
        window->setPlace( rel->getLeft() +
                         ( 1 - _x1 ) * geometryAllocation.getLeft() +
                         _x1 * geometryAllocation.getRight(),
                         rel->getBottom() +
                         ( 1 - _y1 ) * geometryAllocation.getBottom() +
                         _y1 * geometryAllocation.getTop() );
        window->align( _x2, _y2 );
        window->map();

      }

    }

  }

}


void gkg::MenuImplementation::close()
{

  if ( _item >= 0 )
  {

    gkg::MenuItem* menuItem = getItem( _item );
    gkg::TellTaleState* tellTaleState = menuItem->getState();
    if ( tellTaleState && tellTaleState->test( gkg::TellTaleState::Enabled ) )
    {

      tellTaleState->set( gkg::TellTaleState::Active, false );

    }
    gkg::Menu* menu = menuItem->getMenu();
    if ( menu )
    {

      menu->unselect();

    }
    gkg::Window* window = menuItem->getWindow();
    if ( window )
    {

      window->unmap();

    }

  }

}


gkg::Menu* gkg::MenuImplementation::getSelectedMenu() const
{

  gkg::Menu* menu = 0;
  if ( _item >= 0 )
  {

    menu = _items[ _item ]->getMenu();

  }
  return menu;

}


void gkg::MenuImplementation::saveCursor( gkg::Canvas* canvas )
{

  if ( !_savedCursor )
  {

    gkg::Window* window = canvas->getWindow();
    _cursor = window->getCursor();
    _savedCursor = true;
    window->setCursor( gkg::Cursor::getMenu() );

  }

}


void gkg::MenuImplementation::restoreCursor( gkg::Canvas* canvas )
{

  if ( _savedCursor )
  {

    canvas->getWindow()->setCursor( _cursor );
    _savedCursor = false;

  }

}


void gkg::MenuImplementation::grab( gkg::Menu* menu, const gkg::Event& event )
{

  if ( !_grabbed )
  {

    event.grab( menu->getEventHandler() );
    _grabbed = true;

  }

}


void gkg::MenuImplementation::ungrab( gkg::Menu* menu, const gkg::Event& event )
{

  if ( _grabbed )
  {

    event.ungrab( menu->getEventHandler() );
    _grabbed = false;

  }

}
