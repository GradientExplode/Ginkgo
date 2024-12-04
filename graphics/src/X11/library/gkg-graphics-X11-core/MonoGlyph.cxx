#include <gkg-graphics-X11-core/MonoGlyph.h>


gkg::MonoGlyph::MonoGlyph( gkg::Glyph* glyph )
               : gkg::Glyph(),
                 _body( glyph )
{

  if ( _body )
  {

    _body->addReference();

  }

}


gkg::MonoGlyph::~MonoGlyph()
{

  if ( _body )
  {

    _body->removeReference();

  }

}


void gkg::MonoGlyph::setBody( gkg::Glyph* glyph )
{

  if ( glyph )
  {

    glyph->addReference();

  }
  if ( _body )
  {

    _body->undraw();
    if ( _body )
    {

      _body->removeReferenceDeferred();

    }

  }
  _body = glyph;

}


gkg::Glyph* gkg::MonoGlyph::getBody() const
{

  return _body;

}


void 
gkg::MonoGlyph::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _body )
  {

    _body->request( geometryRequisition );

  }
  else
  {

    this->gkg::Glyph::request( geometryRequisition );

  }

}


void 
gkg::MonoGlyph::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  if ( _body )
  {

    _body->allocate( canvas, geometryAllocation, geometryExtension );

  }
  else
  {

    this->gkg::Glyph::allocate( canvas, geometryAllocation, geometryExtension );

  }

}


void 
gkg::MonoGlyph::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _body )
  {

    _body->draw( canvas, geometryAllocation );
  }
  else
  {

    this->gkg::Glyph::draw( canvas, geometryAllocation );

  }

}


void 
gkg::MonoGlyph::print( gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _body )
  {

    _body->print( printer, geometryAllocation );

  }
  else
  {

    this->gkg::Glyph::print( printer, geometryAllocation );

  }

}


void 
gkg::MonoGlyph::pick( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation,
                      int32_t depth,
                      gkg::Hit& hit )
{

  if ( _body )
  {

    _body->pick( canvas, geometryAllocation, depth, hit );

  }
  else
  {

    this->gkg::Glyph::pick( canvas, geometryAllocation, depth, hit );

  }

}


void gkg::MonoGlyph::undraw()
{

  if ( _body )
  {

    _body->undraw();

  }

}


void gkg::MonoGlyph::append( gkg::Glyph* other )
{

  if ( _body )
  {

    _body->append( other );

  }

}


void gkg::MonoGlyph::prepend( gkg::Glyph* other )
{

  if ( _body )
  {

    _body->prepend( other );

  }

}


void gkg::MonoGlyph::insert( gkg::Glyph::Index index, gkg::Glyph* other )
{

  if ( _body )
  {

    _body->insert( index, other );

  }

}


void gkg::MonoGlyph::remove( gkg::Glyph::Index index )
{

  if ( _body )
  {

    _body->remove( index );

  }

}


void gkg::MonoGlyph::replace( gkg::Glyph::Index index, gkg::Glyph* other )
{

  if ( _body )
  {

    _body->replace( index, other );

  }

}


void gkg::MonoGlyph::change( gkg::Glyph::Index index )
{

  if ( _body )
  {

    _body->change( index );

  }

}


gkg::Glyph::Index gkg::MonoGlyph::getCount() const
{

  if ( _body )
  {

    return _body->getCount();

  }
  else
  {

    return this->gkg::Glyph::getCount();

  }

}


gkg::Glyph* gkg::MonoGlyph::getComponent( gkg::Glyph::Index index ) const
{

  if ( _body )
  {

    return _body->getComponent( index );
  }
  else
  {

    return this->gkg::Glyph::getComponent( index );

  }

}


void gkg::MonoGlyph::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  if ( _body )
  {

    return _body->getGeometryAllotment( index, dimensionName,
                                        geometryAllotment );

  }
  else
  {

    return this->gkg::Glyph::getGeometryAllotment( index, dimensionName,
                                                   geometryAllotment );

  }

}
