#ifndef _gkg_dmri_container_OdfCartesianToContinuousField_h
#define _gkg_dmri_container_OdfCartesianToContinuousField_h


#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-dmri-container/OdfCartesianField.h>


namespace gkg
{


class OdfCartesianToContinuousField : public OdfContinuousField
{

  public:

    OdfCartesianToContinuousField( const OdfCartesianField& odfCartesianField,
                                   bool storeItems );
    virtual ~OdfCartesianToContinuousField();

    const OrientationSet& getInputOrientationSet() const;
    const std::vector< float > getBValues() const;

    const OrientationSet& getOutputOrientationSet() const;

    void updateCache( float apertureAngle ) const;

    bool isValid( const Vector3d< float >& site ) const;

  protected:

    OrientationDistributionFunction*
      newItem( const Vector3d< float >& site );

    const OdfCartesianField& _odfCartesianField;

    Vector3d< int32_t > _offset;
    Vector3d< int32_t > _sizes;
    Vector3d< double > _resolution;

};


}


#endif

