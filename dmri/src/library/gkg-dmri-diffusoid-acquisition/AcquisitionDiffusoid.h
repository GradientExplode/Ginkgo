#ifndef _gkg_dmri_diffusoid_acquisition_AcquisitionDiffusoid_h_
#define _gkg_dmri_diffusoid_acquisition_AcquisitionDiffusoid_h_


#include <gkg-dmri-diffusoid/Diffusoid.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class AcquisitionDiffusoid : public Diffusoid
{

  public:

    // for standard estimation on the acquired grid
    AcquisitionDiffusoid( const OrientationSet* orientationSet,
                          const SO3Interpolator* so3Interpolator,
                          const Volume< float >& t2,
                          const Volume< float >& dw,
                          const Vector3d< int32_t >& site );

    // for partial voluming resampled data
    AcquisitionDiffusoid( const OrientationSet* orientationSet,
                          const SO3Interpolator* so3Interpolator,
                          const Volume< float >& t2,
                          const Volume< float >& dw,
                          const Vector3d< float >& site );

    // for standard estimation on the acquired grid with reconstruction matrix
    AcquisitionDiffusoid( const OrientationSet* orientationSet,
                          const SO3Interpolator* so3Interpolator,
                          const Volume< float >& t2,
                          const Volume< float >& dw,
                          const Vector3d< int32_t >& site,
                          const Matrix& reconstructionMatrix );

    // for partial voluming resampled data with reconstruction matrix
    AcquisitionDiffusoid( const OrientationSet* orientationSet,
                          const SO3Interpolator* so3Interpolator,
                          const Volume< float >& t2,
                          const Volume< float >& dw,
                          const Vector3d< float >& site,
                          const Matrix& reconstructionMatrix );
    virtual ~AcquisitionDiffusoid();

};


}


#endif
