#include <gkg-graphics-X11-core/DragBox.h>


gkg::DragBox::DragBox( gkg::Glyph* body, const gkg::Color* color )
             : gkg::Background( body, color ),
               _everDrawn( false )
{
}


gkg::DragBox::~DragBox()
{
}


void gkg::DragBox::setEverDrawn( bool drawn )
{

  _everDrawn = drawn;

}


bool gkg::DragBox::isEverDrawn() const
{

  return _everDrawn;

}

void gkg::DragBox::draw( 
                       gkg::Canvas* canvas, 
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::Background::draw( canvas, geometryAllocation );
  gkg::DragBox* self = ( gkg::DragBox* )this;  // not a beautiful hack!
  self->_everDrawn = true;

}
