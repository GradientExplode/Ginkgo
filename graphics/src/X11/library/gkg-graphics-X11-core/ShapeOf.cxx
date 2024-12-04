#include <gkg-graphics-X11-core/ShapeOf.h>


gkg::ShapeOf::ShapeOf( gkg::Glyph* x, gkg::Glyph* y )
             : gkg::Glyph(),
               _x( x ),
               _y( y )
{

  if ( _x )
  {

    _x->addReference();

  }
  if ( _y )
  {

    _y->addReference();

  }

}


gkg::ShapeOf::~ShapeOf()
{

  if ( _x )
  {

    _x->removeReference();

  }
  if ( _y )
  {

    _y->removeReference();

  }

}


void 
gkg::ShapeOf::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _x == _y )
  {

    _x->request( geometryRequisition );

  }
  else
  {

    gkg::GeometryRequisition req;
    if ( _x )
    {

      _x->request( req );
      geometryRequisition.setRequirementX( req.getRequirementX() );

    }
    if ( _y )
    {

      _y->request( req );
      geometryRequisition.setRequirementY( req.getRequirementY() );

    }

  }

}
