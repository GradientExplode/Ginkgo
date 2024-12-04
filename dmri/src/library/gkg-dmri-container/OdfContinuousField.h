#ifndef _gkg_dmri_container_OdfContinuousField_h_
#define _gkg_dmri_container_OdfContinuousField_h_


#include <gkg-dmri-container/DiffusionContinuousField.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>


namespace gkg
{


class OdfContinuousField : public DiffusionContinuousField<
                                               OrientationDistributionFunction >
{

  public:

    virtual ~OdfContinuousField();

    virtual const OrientationSet& getOutputOrientationSet() const = 0;

    virtual bool isValid( const Vector3d< float >& site ) const = 0;

    virtual void updateCache( float apertureAngle, bool cosine = false ) const;

  protected:

    OdfContinuousField( bool storeItems );

};


}


#endif
