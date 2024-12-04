#include <gkg-graphics-X11-core/VStrut.h>


gkg::VStrut::VStrut( gkg::FloatCoordinate ascent,
                     gkg::FloatCoordinate descent,
                     gkg::FloatCoordinate natural,
                     gkg::FloatCoordinate stretch,
                     gkg::FloatCoordinate shrink )
            : gkg::Glyph(),
              _ascent( ascent ),
              _descent( descent ),
              _natural( natural ),
              _stretch( stretch ),
              _shrink( shrink )
{
}


gkg::VStrut::~VStrut()
{
}


void gkg::VStrut::request( gkg::GeometryRequisition& geometryRequisition) const
{

  gkg::FloatCoordinate height = _ascent + _descent;
  gkg::GeometryRequirement rx( _natural, _stretch, _shrink, 0 );
  gkg::GeometryRequirement ry( height, 0, 0,
                               ( height == 0 ) ? 0 : _descent / height );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}
