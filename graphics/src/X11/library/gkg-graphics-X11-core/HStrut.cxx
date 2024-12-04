#include <gkg-graphics-X11-core/HStrut.h>


gkg::HStrut::HStrut( gkg::FloatCoordinate rightBearing,
                     gkg::FloatCoordinate leftBearing,
                     gkg::FloatCoordinate natural,
                     gkg::FloatCoordinate stretch,
                     gkg::FloatCoordinate shrink )
            : gkg::Glyph(),
              _rightBearing( rightBearing ),
              _leftBearing( leftBearing ),
              _natural( natural ),
              _stretch( stretch ),
              _shrink( shrink )
{
}


gkg::HStrut::~HStrut()
{
}


void 
gkg::HStrut::request( gkg::GeometryRequisition& geometryRequisition) const
{

  gkg::FloatCoordinate width = _rightBearing + _leftBearing;
  gkg::GeometryRequirement rx( width, 0, 0,
                               ( width == 0 ) ? 0 : _leftBearing / width );
  gkg::GeometryRequirement ry( _natural, _stretch, _shrink, 0 );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}
