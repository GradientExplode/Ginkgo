#include <gkg-graphics-X11-core/DragZoneSinkHandler.h>
#include <gkg-graphics-X11-core/DragZoneSinkImplementation.h>


gkg::DragZoneSinkHandler::DragZoneSinkHandler(
                   gkg::DragZoneSinkImplementation* dragZoneSinkImplementation )
                     : _dragZoneSinkImplementation( dragZoneSinkImplementation )
{
}


gkg::DragZoneSinkHandler::~DragZoneSinkHandler()
{
}


bool gkg::DragZoneSinkHandler::event( gkg::Event& event )
{

  return _dragZoneSinkImplementation->event( event );

}
