#include <gkg-anatomist-plugin-control/MouseAction.h>
#include <gkg-anatomist-plugin-control/MouseEvent.h>

#include <anatomist/window/glwidgetmanager.h>


gkg::MouseAction::MouseAction()
                : anatomist::Action(),
                  _mouseEvent( 0 )
{
}


gkg::MouseAction::MouseAction( const gkg::MouseAction& a )
                : anatomist::Action( a ),
                  _mouseEvent( a._mouseEvent )
{
}


anatomist::Action* gkg::MouseAction::creator()
{

  return new gkg::MouseAction();

}


std::string gkg::MouseAction::name() const
{

  return "GkgMouseAction";

}


void gkg::MouseAction::setMouseEvent( gkg::MouseEvent* event )
{

  _mouseEvent.reset( event );

}


void gkg::MouseAction::press( int32_t x, 
                              int32_t y, 
                              int32_t /* globalX */, 
                              int32_t /* globalY */ )
{

  anatomist::View* v = view();
  anatomist::GLWidgetManager* w = 
    dynamic_cast< anatomist::GLWidgetManager* >( v );

  if ( w && !_mouseEvent.isNull() )
  {

    _mouseEvent->press( v->aWindow(), x, y );

  }

}


void gkg::MouseAction::move( int32_t x, 
                             int32_t y, 
                             int32_t /* globalX */, 
                             int32_t /* globalY */ )
{

  anatomist::View* v = view();
  anatomist::GLWidgetManager* w = 
    dynamic_cast< anatomist::GLWidgetManager* >( v );

  if ( w && !_mouseEvent.isNull() )
  {

    _mouseEvent->move( v->aWindow(), x, y );

  }

}


void gkg::MouseAction::release( int32_t x, 
                                int32_t y, 
                                int32_t /* globalX */, 
                                int32_t /* globalY */ )
{

  anatomist::View* v = view();
  anatomist::GLWidgetManager* w = 
    dynamic_cast< anatomist::GLWidgetManager* >( v );

  if ( w && !_mouseEvent.isNull() )
  {

    _mouseEvent->release( v->aWindow(), x, y );

  }

}
