#include <gkg-dmri-container/OdfCartesianToContinuousField.h>
#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-dmri-container/DiffusionContinuousField_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-odf/PartialVolumingOdf.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfCartesianToContinuousField::OdfCartesianToContinuousField(
                                const gkg::OdfCartesianField& odfCartesianField,
                                bool storeItems )
                                   : gkg::OdfContinuousField( storeItems ),
                                     _odfCartesianField( odfCartesianField ),
                                     _offset( odfCartesianField.getOffset() ),
                                     _sizes( odfCartesianField.getSize() ),
                                     _resolution( 
                                             odfCartesianField.getResolution() )
{
}


gkg::OdfCartesianToContinuousField::~OdfCartesianToContinuousField()
{
}


const gkg::OrientationSet& 
gkg::OdfCartesianToContinuousField::getInputOrientationSet() const
{

  try
  {

    return _odfCartesianField.getInputOrientationSet();

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::OdfCartesianToContinuousField::getInputOrientationSet() "
             "const" );

}


const std::vector< float >
gkg::OdfCartesianToContinuousField::getBValues() const
{

  try
  {

    return _odfCartesianField.getBValues();

  }
  GKG_CATCH( "const std::vector< float > "
             "gkg::OdfCartesianToContinuousField::getBValues() const" );

}


const gkg::OrientationSet&
gkg::OdfCartesianToContinuousField::getOutputOrientationSet() const
{

  try
  {

    return _odfCartesianField.getOutputOrientationSet();

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::OdfCartesianToContinuousField::getOutputOrientationSet() "
             "const" );

}


void 
gkg::OdfCartesianToContinuousField::updateCache( float apertureAngle ) const
{

  try
  {

    // Use the first item of the cartesian field
    _odfCartesianField.begin()->second.updateCache( 
                                   _odfCartesianField.getOutputOrientationSet(),
                                   apertureAngle );

  }
  GKG_CATCH( "void gkg::OdfCartesianToContinuousField::"
             "updateCache( float apertureAngle ) const)" );

}


bool gkg::OdfCartesianToContinuousField::isValid(
                                      const gkg::Vector3d< float >& site ) const
{

  try
  {

    int32_t inferiorCoordinateX = ( int32_t )( float )( site.x / 
                                                        _resolution.x );
    int32_t inferiorCoordinateY = ( int32_t )( float )( site.y / 
                                                        _resolution.y );
    int32_t inferiorCoordinateZ = ( int32_t )( float )( site.z / 
                                                        _resolution.z );
    int32_t superiorCoordinateX = inferiorCoordinateX + 1;
    int32_t superiorCoordinateY = inferiorCoordinateY + 1;
    int32_t superiorCoordinateZ = inferiorCoordinateZ + 1;

#if 0
    if ( _odfCartesianField.getItem( inferiorCoordinateX,
                                     inferiorCoordinateY,
                                     inferiorCoordinateZ ) ||
         _odfCartesianField.getItem( inferiorCoordinateX,
                                     inferiorCoordinateY,
                                     superiorCoordinateZ ) ||
         _odfCartesianField.getItem( inferiorCoordinateX,
                                     superiorCoordinateY,
                                     inferiorCoordinateZ ) ||
         _odfCartesianField.getItem( inferiorCoordinateX,
                                     superiorCoordinateY,
                                     superiorCoordinateZ ) ||
         _odfCartesianField.getItem( superiorCoordinateX,
                                     inferiorCoordinateY,
                                     inferiorCoordinateZ ) ||
         _odfCartesianField.getItem( superiorCoordinateX,
                                     inferiorCoordinateY,
                                     superiorCoordinateZ ) ||
         _odfCartesianField.getItem( superiorCoordinateX,
                                     superiorCoordinateY,
                                     inferiorCoordinateZ ) ||
         _odfCartesianField.getItem( superiorCoordinateX,
                                     superiorCoordinateY,
                                     superiorCoordinateZ ) )
    {

      return true;

    }
    return false;
#endif

    int32_t infX = inferiorCoordinateX - _offset.x;
    int32_t infY = inferiorCoordinateY - _offset.y;
    int32_t infZ = inferiorCoordinateZ - _offset.z;
    int32_t supX = infX + 1;
    int32_t supY = infY + 1;
    int32_t supZ = infZ + 1;

    if ( ( infX >= 0 ) &&
         ( infY >= 0 ) &&
         ( infZ >= 0 ) &&
         ( infX < _sizes.x ) &&
         ( infY < _sizes.y ) &&
         ( infZ < _sizes.z ) &&
         ( supX >= 0 ) &&
         ( supY >= 0 ) &&
         ( supZ >= 0 ) &&
         ( supX < _sizes.x ) &&
         ( supY < _sizes.y ) &&
         ( supZ < _sizes.z ) )
    {

      return ( _odfCartesianField.getItemFast( inferiorCoordinateX,
                			       inferiorCoordinateY,
                			       inferiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( inferiorCoordinateX,
                			       inferiorCoordinateY,
                			       superiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( inferiorCoordinateX,
                			       superiorCoordinateY,
                			       inferiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( inferiorCoordinateX,
                			       superiorCoordinateY,
                			       superiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( superiorCoordinateX,
                			       inferiorCoordinateY,
                			       inferiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( superiorCoordinateX,
                			       inferiorCoordinateY,
                			       superiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( superiorCoordinateX,
                			       superiorCoordinateY,
                			       inferiorCoordinateZ ) ||
               _odfCartesianField.getItemFast( superiorCoordinateX,
                			       superiorCoordinateY,
                			       superiorCoordinateZ ) );

    }
    else  if ( ( infX >= 0 ) &&
               ( infY >= 0 ) &&
               ( infZ >= 0 ) &&
               ( infX <= _sizes.x ) &&
               ( infY <= _sizes.y ) &&
               ( infZ <= _sizes.z ) &&
               ( supX >= 0 ) &&
               ( supY >= 0 ) &&
               ( supZ >= 0 ) &&
               ( supX <= _sizes.x ) &&
               ( supY <= _sizes.y ) &&
               ( supZ <= _sizes.z ) )
    {

      return ( _odfCartesianField.getItem( inferiorCoordinateX,
                			   inferiorCoordinateY,
                			   inferiorCoordinateZ ) ||
               _odfCartesianField.getItem( inferiorCoordinateX,
                			   inferiorCoordinateY,
                			   superiorCoordinateZ ) ||
               _odfCartesianField.getItem( inferiorCoordinateX,
                			   superiorCoordinateY,
                			   inferiorCoordinateZ ) ||
               _odfCartesianField.getItem( inferiorCoordinateX,
                			   superiorCoordinateY,
                			   superiorCoordinateZ ) ||
               _odfCartesianField.getItem( superiorCoordinateX,
                			   inferiorCoordinateY,
                			   inferiorCoordinateZ ) ||
               _odfCartesianField.getItem( superiorCoordinateX,
                			   inferiorCoordinateY,
                			   superiorCoordinateZ ) ||
               _odfCartesianField.getItem( superiorCoordinateX,
                			   superiorCoordinateY,
                			   inferiorCoordinateZ ) ||
               _odfCartesianField.getItem( superiorCoordinateX,
                			   superiorCoordinateY,
                			   superiorCoordinateZ ) );

    }

    return false;

  }
  GKG_CATCH( "bool gkg::OdfCartesianToContinuousField::isValid( "
             "const gkg::Vector3d< float >& site ) const" );

}


gkg::OrientationDistributionFunction*
gkg::OdfCartesianToContinuousField::newItem( const gkg::Vector3d< float >& site )
{

  try
  {

    return new gkg::PartialVolumingOdf( _odfCartesianField, 
                                        site,
                                        _sizes,
                                        _resolution );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction* "
             "gkg::OdfCartesianToContinuousField::newItem( "
             "const gkg::Vector3d< float >& site )" );

}
