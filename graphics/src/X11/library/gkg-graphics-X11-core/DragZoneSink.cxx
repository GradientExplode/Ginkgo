#include <gkg-graphics-X11-core/DragZoneSink.h>
#include <gkg-graphics-X11-core/DragZoneSinkImplementation.h>
#include <gkg-graphics-X11-core/DragZoneSinkHandler.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::DragZoneSink::DragZoneSink( gkg::Glyph* glyph )
                  : gkg::DragZone( glyph )
{

  _dragZoneSinkImplementation = 
  gkg::getGUIImplementationFactory()->createDragZoneSinkImplementation( this );

}


gkg::DragZoneSink::~DragZoneSink()
{

  delete _dragZoneSinkImplementation;

}


void gkg::DragZoneSink::drop( gkg::Event& event, const char* data, 
                              int32_t length )
{

  _dragZoneSinkImplementation->drop( event, data, length );

}


void gkg::DragZoneSink::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::DragZone::draw( canvas, geometryAllocation );
  _dragZoneSinkImplementation->draw( canvas );

}


void gkg::DragZoneSink::pick( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              int32_t depth,
                              gkg::Hit& hit )
{

  gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
  _dragZoneSinkImplementation->pick( depth, hit );

}


bool gkg::DragZoneSink::event( gkg::Event& event )
{

  return _dragZoneSinkImplementation->event( event );

}


gkg::DragZoneSinkImplementation* gkg::DragZoneSink::getImplementation() const
{

  return _dragZoneSinkImplementation;

}
