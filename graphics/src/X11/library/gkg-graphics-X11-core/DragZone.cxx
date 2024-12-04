#include <gkg-graphics-X11-core/DragZone.h>
#include <gkg-graphics-X11-core/DragZoneImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::DragZone::DragZone( gkg::Glyph* glyph )
              : gkg::MonoGlyph( glyph )
{

  _dragZoneImplementation = 
    gkg::getGUIImplementationFactory()->createDragZoneImplementation( this );

}


gkg::DragZone::~DragZone()
{

  delete _dragZoneImplementation;

}


void gkg::DragZone::setSensitive( bool sensitive )
{

  _dragZoneImplementation->setSensitive( sensitive );

}

bool gkg::DragZone::isSensitive() const
{

  return _dragZoneImplementation->isSensitive();

}


void gkg::DragZone::enter( gkg::Event&, const char*, int32_t )
{
}


void gkg::DragZone::motion( gkg::Event& )
{
}


void gkg::DragZone::leave( gkg::Event& )
{
}


void gkg::DragZone::allocate( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  gkg::MonoGlyph::allocate( canvas, geometryAllocation, geometryExtension );
  _dragZoneImplementation->allocate( canvas,
                                     geometryAllocation,
                                     geometryExtension );

}


void gkg::DragZone::pick( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          int32_t depth,
                          gkg::Hit& hit )
{

  const gkg::Event* event = hit.getEvent();
  if ( event &&
       ( hit.getLeft() <= geometryAllocation.getRight() ) &&
       ( hit.getRight() >= geometryAllocation.getLeft() ) &&
       ( hit.getBottom() <= geometryAllocation.getTop() ) &&
       ( hit.getTop() >= geometryAllocation.getBottom() ) &&
       _dragZoneImplementation->caught( *event ) )
  {

    hit.setTarget( depth, this, 0, _dragZoneImplementation->getEventHandler() );

  }
  gkg::MonoGlyph::pick( canvas, geometryAllocation, depth + 1, hit );

}


gkg::DragZoneImplementation* gkg::DragZone::getImplementation() const
{

  return _dragZoneImplementation;

}
