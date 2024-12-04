#include <gkg-graphics-X11-core/LayoutLayer.h>


gkg::LayoutLayer::LayoutLayer( gkg::Glyph* between,
                                  gkg::Glyph* under,
                                  gkg::Glyph* over )
                    : gkg::MonoGlyph( between ),
                      _under( under ),
                      _over( over )
{

  if ( _under )
  {

    _under->addReference();

  }
  if ( _over )
  {

    _over->addReference();

  }

}


gkg::LayoutLayer::~LayoutLayer()
{

  if ( _under )
  {

    _under->removeReference();

  }
  if ( _over )
  {

    _over->removeReference();

  }

}


void gkg::LayoutLayer::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  if ( _under )
  {

    _under->allocate( canvas, geometryAllocation, geometryExtension );

  }
  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
  if ( _over )
  {

    _over->allocate( canvas, geometryAllocation, geometryExtension );

  }

}


void gkg::LayoutLayer::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _under )
  {

    _under->draw( canvas, geometryAllocation );

  }
  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );
  if ( _over )
  {

    _over->draw( canvas, geometryAllocation );

  }

}


void gkg::LayoutLayer::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _under )
  {

    _under->print( printer, geometryAllocation );

  }
  this->gkg::MonoGlyph::print( printer, geometryAllocation );
  if ( _over )
  {

    _over->print( printer, geometryAllocation );

  }

}

void gkg::LayoutLayer::pick( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             int32_t depth,
                             gkg::Hit& hit )
{

  if ( _under )
  {

    _under->pick( canvas, geometryAllocation, depth, hit );

  }
  this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
  if ( _over )
  {

    _over->pick( canvas, geometryAllocation, depth, hit );

  }

}


void gkg::LayoutLayer::undraw()
{

  if ( _under )
  {

    _under->undraw();

  }
  this->gkg::MonoGlyph::undraw();
  if ( _over )
  {

    _over->undraw();

  }

}
