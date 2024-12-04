#include <gkg-graphics-X11-core/DragZoneSink.h>
#include <gkg-graphics-X11-core/DragZoneSinkImplementation.h>
#include <gkg-graphics-X11-core/DragZoneSinkHandler.h>


gkg::DragZoneSinkImplementation::DragZoneSinkImplementation(
                                              gkg::DragZoneSink * dragZoneSink )
                         : _target( dragZoneSink ),
                           _dragPublished( false ),
                           _eventHandler( new gkg::DragZoneSinkHandler( this ) )
{

  if ( _eventHandler )
  {

    _eventHandler->addReference();

  }

}


gkg::DragZoneSinkImplementation::~DragZoneSinkImplementation()
{

  if ( _eventHandler )
  {

    _eventHandler->removeReference();

  }

}


gkg::DragZoneSink* gkg::DragZoneSinkImplementation::getTarget() const
{

  return _target;

}


gkg::EventHandler* gkg::DragZoneSinkImplementation::getEventHandler() const
{

 return _eventHandler;

}


bool gkg::DragZoneSinkImplementation::isDragPublished() const
{

  return _dragPublished;

}
