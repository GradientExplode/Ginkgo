#include <gkg-graphics-X11-core/Menu.h>
#include <gkg-graphics-X11-core/MenuImplementation.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/PopupWindow.h>
#include <gkg-core-pattern/Observer.h>


//
// class MenuItem
//

gkg::MenuItem::MenuItem( gkg::Glyph* glyph,
                         gkg::TellTaleState* tellTaleState )
              : gkg::Observer(),
                _action( 0 ),
                _menu( 0 ),
                _window( 0 )
{

  initialize( glyph, tellTaleState );

}


gkg::MenuItem::MenuItem( gkg::Glyph* glyph,
                         gkg::TellTaleState* tellTaleState,
                         gkg::Action* action )
              : gkg::Observer(),
                _action( action ),
                _menu( 0 ),
                _window( 0 )
{

  initialize( glyph, tellTaleState );
  if ( _action )
  {

    _action->addReference();

  }

}


gkg::MenuItem::MenuItem( gkg::Glyph* glyph,
                         gkg::TellTaleState* tellTaleState,
                         gkg::Menu* menu,
                         gkg::Window* window )
              : gkg::Observer(),
                _action( 0 ),
                _menu( menu ),
                _window( window )
{

  initialize( glyph, tellTaleState );
  if ( _menu )
  {

    _menu->addReference();

  }
  if ( !_window )
  {

    _window = new gkg::PopupWindow( _menu );

  }
  _window->setCursor( gkg::Cursor::getMenu() );

}


gkg::MenuItem::~MenuItem()
{

  _state->removeObserver( this );
  if ( _state )
  {

    _state->removeReference();

  }
  if ( _action )
  {

    _action->removeReference();

  }
  if ( _menu )
  {

    _menu->removeReference();

  }
  if ( _patch )
  {

    _patch->removeReference();

  }
  delete _window;

}


gkg::Glyph* gkg::MenuItem::getBody() const
{

  return _patch;

}


gkg::TellTaleState* gkg::MenuItem::getState() const
{

  return _state;

}


void gkg::MenuItem::setAction( gkg::Action* action )
{

  if ( action )
  {

    action->addReference();

  }
  if ( _action )
  {

    _action->removeReference();

  }
  _action = action;

}


gkg::Action* gkg::MenuItem::getAction() const
{

  return _action;

}


void gkg::MenuItem::setMenu( gkg::Menu* menu, gkg::Window* window )
{

  if ( menu )
  {

    menu->addReference();

  }
  if ( _menu )
  {

    _menu->removeReference();

  }
  _menu = menu;
  if ( !window )
  {

    _window = new gkg::PopupWindow( _menu );

  }
  _window->setCursor( gkg::Cursor::getMenu() );

}


gkg::Menu* gkg::MenuItem::getMenu() const
{

  return _menu;

}


gkg::Window* gkg::MenuItem::getWindow() const
{

  return _window;

}


gkg::Patch* gkg::MenuItem::getPatch() const
{

  return _patch;

}


void gkg::MenuItem::update( const gkg::Observable*, void* )
{

  _patch->redraw();

}


void gkg::MenuItem::initialize( gkg::Glyph* glyph,
                                gkg::TellTaleState* tellTaleState )
{

  _patch = new gkg::Patch( glyph );
  if ( _patch )
  {

    _patch->addReference();

  }
  _state = tellTaleState;
  if ( _state )
  {

    _state->addReference();

  }
  _state->addObserver( this );

}


//
// class Menu
//

gkg::Menu::Menu( gkg::Glyph* glyph,
                 gkg::Style* style,
                 float x1, float y1,
                 float x2, float y2 )
          : gkg::InputHandler( glyph, style )
{

  _menuImplementation = new gkg::MenuImplementation( this, x1, y1, x2, y2 );

}


gkg::Menu::~Menu()
{

  delete _menuImplementation;

}


void gkg::Menu::appendItem( gkg::MenuItem* menuItem )
{

  _menuImplementation->appendItem( menuItem );
  append( menuItem->getBody() );

}


void gkg::Menu::prependItem( gkg::MenuItem* menuItem )
{

  _menuImplementation->prependItem( menuItem );
  prepend( menuItem->getBody() );

}


void gkg::Menu::insertItem( gkg::Glyph::Index index, gkg::MenuItem* menuItem )
{

  _menuImplementation->insertItem( index, menuItem );
  insert( index, menuItem->getBody() );

}


void gkg::Menu::removeItem( gkg::Glyph::Index index )
{

  _menuImplementation->removeItem( index );
  remove( index );

}


void gkg::Menu::replaceItem( gkg::Glyph::Index index, gkg::MenuItem* menuItem )
{

  if ( _menuImplementation->replaceItem( index, menuItem ) )
  {

    replace( index, menuItem->getBody() );

  }

}


gkg::Glyph::Index gkg::Menu::getItemCount() const
{

  return _menuImplementation->getItemCount();

}


gkg::MenuItem* gkg::Menu::getItem( gkg::Glyph::Index index ) const
{

  return _menuImplementation->getItem( index );

}


void gkg::Menu::select( gkg::Glyph::Index index )
{

  _menuImplementation->select( index );

}


gkg::Glyph::Index gkg::Menu::getSelected() const
{

  return _menuImplementation->getSelected();

}


void gkg::Menu::unselect()
{

  _menuImplementation->unselect();

}


void gkg::Menu::press( const gkg::Event& event )
{

  gkg::Canvas* canvas = getCanvas();
  if ( canvas )
  {

    _menuImplementation->saveCursor( canvas );
    drag( event );

  }

}


void gkg::Menu::drag( const gkg::Event& event )
{

  gkg::Canvas* canvas = getCanvas();
  if ( !canvas )
  {

    unselect();
    return;

  }
  gkg::Window* window = canvas->getWindow();
  gkg::Hit hit( event.getPointerRootX() - window->getLeft(),
                event.getPointerRootY() - window->getBottom() );
  pick( canvas, getGeometryAllocation(), 0, hit );
  if ( hit.hasAny() )
  {

    gkg::Glyph::Index index = hit.getIndex( 0 );
    gkg::Menu* subMenu = getItem( index )->getMenu();
    if ( subMenu )
    {

      subMenu->unselect();

    }
    select( index );

  }
  else
  {

    gkg::Glyph::Index index = getSelected();
    if ( index >= 0 )
    {

      gkg::Menu* subMenu = getItem( index )->getMenu();
      if ( subMenu )
      {

        subMenu->drag( event );
        return;

      }

    }
    unselect();

  }

}


void gkg::Menu::release( const gkg::Event& event )
{

  gkg::Menu* menu = this;
  gkg::Glyph::Index index;
  for ( ; ; )
  {

    index = menu->getSelected();
    if ( index < 0 )
    {

      break;

    }
    gkg::Menu* subMenu = menu->getItem( index )->getMenu();
    if ( !subMenu )
    {

      break;

    }
    menu = subMenu;

  }

  if ( index >= 0 )
  {

    gkg::Glyph::Index topIndex = getSelected();
    gkg::TellTaleState* topTellTaleState = getItem( topIndex )->getState();
    topTellTaleState->set( gkg::TellTaleState::Running, true );
    _menuImplementation->ungrab( this, event );
    gkg::Canvas* canvas = getCanvas();
    if ( canvas )
    {

      _menuImplementation->restoreCursor( canvas );

    }
    unselect();
    gkg::MenuItem* menuItem = menu->getItem( index );
    gkg::TellTaleState* tellTaleState = menuItem->getState();
    gkg::Action* action = menuItem->getAction();
    if ( tellTaleState && tellTaleState->test( gkg::TellTaleState::Enabled ) )
    {

      bool act = !tellTaleState->test( gkg::TellTaleState::Chosen );
      if ( tellTaleState->test( gkg::TellTaleState::Toggle ) )
      {

        tellTaleState->set( gkg::TellTaleState::Chosen, act );
        act = true;

      }
      else if ( tellTaleState->test( gkg::TellTaleState::Choosable ) )
      {

        tellTaleState->set( gkg::TellTaleState::Chosen, true );

      }

      if ( act && action )
      {

        action->execute();

      }

    }
    topTellTaleState->set( gkg::TellTaleState::Running, false );

  }
  else
  {

    // if we hit an item with a submenu, then we leave the submenu open 
    // (with item 0 selected), grab, and return
    gkg::Canvas* canvas;
    gkg::Menu* subMenu;
    for ( menu = this, canvas = getCanvas();
          canvas != 0; menu = subMenu, canvas = menu->getCanvas() )
    {

      gkg::Window* window = canvas->getWindow();
      gkg::Hit hit( event.getPointerRootX() - window->getLeft(),
                    event.getPointerRootY() - window->getBottom() );
      menu->pick( canvas, menu->getGeometryAllocation(), 0, hit );
      if ( hit.hasAny() )
      {

        menu = menu->getItem( hit.getIndex( 0 ) )->getMenu();
        if ( menu )
        {

          menu->select( 0 );
          _menuImplementation->grab( this, event );
          return;

        }
        break;

      }
      subMenu = menu->getImplementation()->getSelectedMenu();
      if ( !subMenu )
      {

        break;

      }

    }
    _menuImplementation->ungrab( this, event );
    canvas = getCanvas();
    if ( canvas )
    {

      _menuImplementation->restoreCursor( canvas );

    }
    unselect();

  }

}


void gkg::Menu::open()
{

  _menuImplementation->open();

}

void gkg::Menu::close() 
{

  _menuImplementation->close();

}


gkg::MenuImplementation* gkg::Menu::getImplementation() const
{

  return _menuImplementation;

}
