#include <gkg-graphics-X11-core/DragZoneHandler.h>
#include <gkg-graphics-X11-core/DragZoneImplementation.h>


gkg::DragZoneHandler::DragZoneHandler(
                           gkg::DragZoneImplementation* dragZoneImplementation )
                     : _dragZoneImplementation( dragZoneImplementation )
{
}


gkg::DragZoneHandler::~DragZoneHandler()
{
}


bool gkg::DragZoneHandler::event( gkg::Event& event )
{

  return _dragZoneImplementation->event( event );

}
