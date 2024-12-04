#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Canvas.h>


gkg::Patch::Patch( gkg::Glyph* body )
           : gkg::MonoGlyph( body ),
             _canvas( 0 )
{
}


gkg::Patch::~Patch()
{
}


gkg::Canvas* gkg::Patch::getCanvas() const
{

  return _canvas;

}


const gkg::Transformer& gkg::Patch::getTransformer() const
{

  return _transformer;

}


const gkg::GeometryAllocation& gkg::Patch::getGeometryAllocation() const
{

  return _geometryAllocation;

}


const gkg::GeometryExtension& gkg::Patch::getGeometryExtension() const
{

  return _geometryExtension;

}


void gkg::Patch::allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  _canvas = canvas;
  _transformer = canvas->getTransformer();
  _geometryAllocation = geometryAllocation;
  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
  _geometryExtension = geometryExtension;

}


void gkg::Patch::draw( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( canvas->isDamaged( _geometryExtension ) )
  {

    this->gkg::MonoGlyph::draw( canvas, geometryAllocation );

  }

}


void gkg::Patch::undraw()
{

  this->gkg::MonoGlyph::undraw();
  _canvas = 0;

}


void gkg::Patch::redraw() const
{

  if ( _canvas )
  {

    _canvas->setDamage( _geometryExtension );

  }

}


void gkg::Patch::reallocate()
{

  if ( _canvas )
  {

    // remove these two lines when doc otherwise works
    gkg::GeometryRequisition s;
    request( s );

    _canvas->setDamage( _geometryExtension ); // added by C. Poupon
                                              // force update of the whole
                                              // previous patch region
    _canvas->pushTransform();
    _canvas->setTransformer( _transformer );
    _geometryExtension.clear();
    allocate( _canvas, _geometryAllocation, _geometryExtension );
    _canvas->popTransform();

  }

}


void gkg::Patch::repick( int32_t depth, gkg::Hit& hit )
{

  if ( _canvas )
  {

    _canvas->pushTransform();
    _canvas->setTransformer( _transformer );
    pick( _canvas, _geometryAllocation, depth, hit );
    _canvas->popTransform();

  }

}


