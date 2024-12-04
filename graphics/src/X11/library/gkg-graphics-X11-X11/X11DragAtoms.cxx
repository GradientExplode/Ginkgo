#include <gkg-graphics-X11-X11/X11DragAtoms.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>


gkg::X11DragAtoms::X11DragAtoms()
                  : _dropUid( 0 ),
                    _xDisplay( 0 ),
                    _enter( 0 ),
                    _motion( 0 ),
                    _leave( 0 ),
                    _drop( 0 ),
                    _drag( 0 )

{
}


gkg::X11DragAtoms::~X11DragAtoms()
{
}


bool gkg::X11DragAtoms::enter( const XEvent& xEvent )
{

  if ( xEvent.type != ClientMessage )
  {

    return false;

  }
  cache( xEvent.xclient.display );
  return _enter && xEvent.xclient.message_type == _enter;

}

bool gkg::X11DragAtoms::motion( const XEvent& xEvent )
{

  if ( xEvent.type != ClientMessage )
  {

    return false;

  }
  cache( xEvent.xclient.display );
  return _motion && ( xEvent.xclient.message_type == _motion );

}


bool gkg::X11DragAtoms::leave( const XEvent& xEvent )
{

  if ( xEvent.type != ClientMessage )
  {

    return false;

  }
  cache( xEvent.xclient.display );
  return _leave && ( xEvent.xclient.message_type == _leave );

}


bool gkg::X11DragAtoms::drop( const XEvent& xEvent )
{

  if ( xEvent.type != ClientMessage )
  {

    return false;

  }
  cache( xEvent.xclient.display );
  return _drop && ( xEvent.xclient.message_type == _drop );

}


bool gkg::X11DragAtoms::enter( const Event& event )
{

  gkg::X11EventImplementation* x11EventImpl =
    gkg::X11EventImplementation::getImplementation( ( gkg::Event* )&event );
  return enter( x11EventImpl->getXEvent() );

}


bool gkg::X11DragAtoms::motion( const Event& event )
{

  gkg::X11EventImplementation* x11EventImpl =
    gkg::X11EventImplementation::getImplementation( ( gkg::Event* )&event );
  return motion( x11EventImpl->getXEvent() );

}


bool gkg::X11DragAtoms::leave( const Event& event )
{

  gkg::X11EventImplementation* x11EventImpl =
    gkg::X11EventImplementation::getImplementation( ( gkg::Event* )&event );
  return leave( x11EventImpl->getXEvent() );

}


bool gkg::X11DragAtoms::drop( const Event& event )
{

  gkg::X11EventImplementation* x11EventImpl =
    gkg::X11EventImplementation::getImplementation( ( gkg::Event* )&event );
  return drop( x11EventImpl->getXEvent() );

}


Atom gkg::X11DragAtoms::enter( XDisplay* xDisplay )
{

  cache( xDisplay );
  return _enter;

}


Atom gkg::X11DragAtoms::motion( XDisplay* xDisplay )
{

  cache( xDisplay );
  return _motion;

}


Atom gkg::X11DragAtoms::leave( XDisplay* xDisplay )
{

  cache( xDisplay );
  return _leave;

}


Atom gkg::X11DragAtoms::drop( XDisplay* xDisplay )
{

  cache( xDisplay );
  return _drop;

}


Atom gkg::X11DragAtoms::drag( XDisplay* xDisplay )
{

  cache( xDisplay );
  return _drag;

}


const char* gkg::X11DragAtoms::getEnterName() const
{

  return "GKG_GUI_ENTER";

}


const char* gkg::X11DragAtoms::getMotionName() const
{

  return "GKG_GUI_MOTION";

}


const char* gkg::X11DragAtoms::getLeaveName() const
{

  return "GKG_GUI_LEAVE";

}


const char* gkg::X11DragAtoms::getDropName() const
{

  return "GKG_GUI_DROP";

}


const char* gkg::X11DragAtoms::getDragName() const
{

  return "GKG_GUI_DRAG";

}


int32_t gkg::X11DragAtoms::getDropUid() const
{

  return _dropUid;

}


void gkg::X11DragAtoms::incDropUid()
{

  ++ _dropUid;

}


void gkg::X11DragAtoms::cache( XDisplay* xDisplay )
{

  if ( _xDisplay == xDisplay )
  {

    return;

  }

  _xDisplay = xDisplay;
  _enter  = XInternAtom( xDisplay, getEnterName(), False );
  _motion = XInternAtom( xDisplay, getMotionName(), False );
  _leave  = XInternAtom( xDisplay, getLeaveName(), False );
  _drop   = XInternAtom( xDisplay, getDropName(), False );
  _drag   = XInternAtom( xDisplay, getDragName(), False );

}


