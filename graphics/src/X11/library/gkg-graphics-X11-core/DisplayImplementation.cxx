#include <cstdlib>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-graphics-X11-core/GrabInfo.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/SelectionManager.h>
#include <gkg-graphics-X11-core/SelectionManagerImplementation.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/EventHandler.h>


gkg::DisplayImplementation::DisplayImplementation(
                                                 gkg::Display* display,
                                                 const std::string& /*device*/ )
                           : _target( display ),
                             _defaultVisual( 0 ),
                             _pixel( 0 ),
                             _point( 0 ),
                             _width( 0 ),
                             _height( 0 ),
                             _pixelWidth( 0 ),
                             _pixelHeight( 0 ),
                             _style( 0 )
{
}


gkg::DisplayImplementation::~DisplayImplementation()
{

  if ( _style )
  {

    _style->removeReferenceDeferred();

  }
  std::list< gkg::SelectionManager* >::iterator s = _selections.begin(),
                                                se = _selections.end();
  while ( s != se )
  {

    delete *s;
    ++ s;

  }
  _selections.clear();
  _damagedWindows.clear();
  _grabbers.clear();

}


gkg::Display* gkg::DisplayImplementation::getTarget() const
{

  return _target;

}


const std::list< gkg::Visual* >& gkg::DisplayImplementation::getVisuals() const
{

  return _visuals;

}


void gkg::DisplayImplementation::addVisual( gkg::Visual* visual )
{

  if ( visual )
  {

    _visuals.push_back( visual );

  }

}


gkg::Visual* gkg::DisplayImplementation::getDefaultVisual() const
{

  return _defaultVisual;

}


gkg::FloatCoordinate gkg::DisplayImplementation::getWidth() const
{

  return _width;

}


gkg::FloatCoordinate gkg::DisplayImplementation::getHeight() const
{

  return _height;

}


gkg::PixelCoordinate gkg::DisplayImplementation::getPixelWidth() const
{

  return _pixelWidth;

}


gkg::PixelCoordinate gkg::DisplayImplementation::getPixelHeight() const
{

  return _pixelHeight;

}


gkg::PixelCoordinate gkg::DisplayImplementation::toPixelCoordinate( 
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return ( gkg::PixelCoordinate )( floatCoordinate * _point + 
                                   ( ( floatCoordinate > 0 ) ? 0.5 : -0.5 ) );

}


gkg::FloatCoordinate gkg::DisplayImplementation::toFloatCoordinate(
                                    gkg::PixelCoordinate pixelCoordinate ) const
{

  return ( gkg::FloatCoordinate )pixelCoordinate * _pixel;

}


gkg::Style* gkg::DisplayImplementation::getStyle() const
{

  return _style;

}


void gkg::DisplayImplementation::repair()
{

  std::list< gkg::Window* >::iterator w = _damagedWindows.begin(),
                                      we = _damagedWindows.end();

  while ( w != we )
  {

    ( *w )->repair();
    ++ w;

  }
  _damagedWindows.clear();

}


// Add a handler to the grabber list.  The handler is referenced to ensure
// that it is not deallocated while on the list.
void gkg::DisplayImplementation::grab( gkg::Window* window,
                                       gkg::EventHandler* eventHandler )
{

  if ( eventHandler )
  {

    eventHandler->addReference();

  }
  _grabbers.push_front( gkg::GrabInfo( window, eventHandler ) );

}


// Remove a handler from the grabber list.
// This function has no effect if the handler is not presently on the list.
// If the handler is on the list, it is unreferenced.
void gkg::DisplayImplementation::ungrab( gkg::EventHandler* eventHandler,
                                         bool all )
{

  std::list< gkg::GrabInfo >::iterator g = _grabbers.begin();
  while ( g != _grabbers.end() )
  {

    if ( g->eventHandler == eventHandler )
    {

      g = _grabbers.erase( g );
      if ( eventHandler )
      {

        eventHandler->removeReference();

      }
      if ( !all )
      {

        break;

      }

    }
    ++ g;

  }

}


// return the most recent grabber, or nil if the list is empty.
gkg::EventHandler* gkg::DisplayImplementation::getGrabber() const
{

  return _grabbers.empty() ? 0 : _grabbers.front().eventHandler;

}


// check whether a given handler is on the grabber list.
bool gkg::DisplayImplementation::isGrabbing(
                                         gkg::EventHandler* eventHandler ) const
{

  std::list< gkg::GrabInfo >::const_iterator g = _grabbers.begin(),
                                             ge = _grabbers.end();
  while ( g != ge )
  {

    if ( g->eventHandler == eventHandler )
    {

      return true;

    }
    ++ g;

  }
  return false;

}


gkg::SelectionManager* gkg::DisplayImplementation::getPrimarySelection()
{

  return findSelection( "PRIMARY" );

}


gkg::SelectionManager* gkg::DisplayImplementation::getSecondarySelection()
{

  return findSelection( "SECONDARY" );

}


gkg::SelectionManager* gkg::DisplayImplementation::getClipboardSelection()
{

  return findSelection( "CLIPBOARD" );

}


gkg::SelectionManager* 
gkg::DisplayImplementation::findSelection( const std::string& name )
{

  std::list< gkg::SelectionManager* >::const_iterator s= _selections.begin(),
                                                      se = _selections.end();
  while ( s != se )
  {

    if ( ( *s )->getImplementation()->getName() == name )
    {

      return *s;

    }
    ++ s;

  }

  gkg::SelectionManager* 
    selection = new gkg::SelectionManager( getTarget(), name );
  _selections.push_back( selection );
  return selection;

}


void gkg::DisplayImplementation::addWindow( gkg::Window* )
{
}


void gkg::DisplayImplementation::removeWindow( gkg::Window* window )
{

  removeFromLists( window );

}


// Compute size of a pixel in printer points.  If the "dpi" attribute
// is specified, then we use it as dots per inch and convert to points.
// Otherwise we use font metrics, not the (alleged) screen size,
// because applications really care about how things measure
// with respect to text.  The default assumes that fonts are designed
// for 75 dots/inch and printer points are 72.0 pts/inch.
void gkg::DisplayImplementation::setDpi()
{

  std::string stringDpi;
  if ( _style->findAttribute( "dpi", stringDpi ) )
  {

    int32_t dpi;
    if ( gkg::StringConverter::toScalar( stringDpi, dpi ) && dpi )
    {

      _pixel = 72.0 / ( float )dpi;

    }

  }
  else
  {

    _pixel = 72.0 / 75.0;

  }

}


// Add a window to the damage list.
void gkg::DisplayImplementation::needsRepair( gkg::Window* window )
{

  _damagedWindows.push_back( window );

}


// Remove a window from relevant display tables.
void gkg::DisplayImplementation::removeFromLists( gkg::Window* window )
{

  std::list< gkg::Window* >::iterator w = _damagedWindows.begin(),
                                      we = _damagedWindows.end();
  while ( w != we )
  {

    if ( *w == window )
    {

      _damagedWindows.erase( w );
      break;

    }
    ++ w;

  }

  // no easy way to delete multiple items during a single list traversal; sigh
  bool done;
  do
  {

    done = true;
    std::list< gkg::GrabInfo >::iterator g = _grabbers.begin(),
                                         ge = _grabbers.end();
    while ( g != ge )
    {

      if ( g->window == window )
      {

        _grabbers.erase( g );
        done = false;
        break;

      }
      ++ g;

    }

  }
  while ( !done );

}


void gkg::DisplayImplementation::referenceStyle( gkg::Style* style )
{

  if ( style )
  {

    style->addReference();

  }
  if ( _style )
  {

    _style->removeReference();

  }
  _style = style;

}
