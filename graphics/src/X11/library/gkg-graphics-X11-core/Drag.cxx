#include <gkg-graphics-X11-core/Drag.h>
#include <gkg-graphics-X11-core/DragImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::Drag::Drag( gkg::Glyph* glyph )
          : gkg::MonoGlyph( glyph )
{

  _dragImplementation =
    gkg::getGUIImplementationFactory()->createDragImplementation( this );

}


gkg::Drag::~Drag()
{

  delete _dragImplementation;

}


void gkg::Drag::setDragable( bool dragable )
{

  _dragImplementation->setDragable( dragable );

}


bool gkg::Drag::isDragable() const
{

  return _dragImplementation->isDragable();

}


void gkg::Drag::getDragType( const char*& value, int32_t& length )
{

  value = 0;
  length = 0;

}


void gkg::Drag::getDragOffset( gkg::Event& event, int32_t& dx, int32_t& dy )
{

  _dragImplementation->getDragOffset( event, dx, dy );

}


bool gkg::Drag::caught( const gkg::Event& event ) const
{

  return ( _dragImplementation->isDragable() &&
           ( event.getType() == gkg::Event::Down ) &&
           event.getPointerButton() == Event::Middle );

}


bool gkg::Drag::commit( const gkg::Event& event ) const
{

  return event.getType() == gkg::Event::Up;

}


bool gkg::Drag::abort( const gkg::Event& event ) const
{

  return event.getType() == gkg::Event::Down;

}


gkg::DragImplementation* gkg::Drag::getImplementation() const
{

  return _dragImplementation;

}


void gkg::Drag::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  _dragImplementation->allocate( geometryAllocation );
  gkg::MonoGlyph::allocate( canvas, geometryAllocation, geometryExtension );

}


void gkg::Drag::pick( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation,
                      int32_t depth,
                      gkg::Hit& hit )
{

  const Event* event = hit.getEvent();
  if ( event &&
       ( hit.getLeft() <= geometryAllocation.getRight() ) &&
       ( hit.getRight() >= geometryAllocation.getLeft() ) &&
       ( hit.getBottom() <= hit.getTop() ) &&
       ( hit.getTop() >= geometryAllocation.getBottom() ) &&
       caught( *event ) )
  {

    hit.setTarget( depth, this, 0, _dragImplementation->getEventHandler() );

  }
  gkg::MonoGlyph::pick( canvas, geometryAllocation, depth + 1, hit );

}
