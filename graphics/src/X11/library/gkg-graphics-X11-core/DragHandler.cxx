#include <gkg-graphics-X11-core/DragHandler.h>
#include <gkg-graphics-X11-core/DragImplementation.h>


gkg::DragHandler::DragHandler( gkg::DragImplementation* dragImplementation )
                 : _dragImplementation( dragImplementation )
{
}


gkg::DragHandler::~DragHandler()
{
}


bool gkg::DragHandler::event( gkg::Event& event )
{

  return _dragImplementation->event( event );

}
