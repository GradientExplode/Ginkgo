#ifndef _gkg_dmri_diffusoid_acquisition_AcquisitionDiffusoidCartesianField_h_
#define _gkg_dmri_diffusoid_acquisition_AcquisitionDiffusoidCartesianField_h_


#include <gkg-dmri-container/DiffusoidCartesianField.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class AcquisitionDiffusoidCartesianField : public DiffusoidCartesianField
{

  public:

    AcquisitionDiffusoidCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const OrientationSet* outputOrientationSet,
                    const SO3Interpolator* so3interpolator,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    AcquisitionDiffusoidCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< float >& boundingBox,
                    const Vector3d< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const SO3Interpolator* so3interpolator,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    AcquisitionDiffusoidCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< int32_t >& boundingBox,
                    const OrientationSet* outputOrientationSet,
                    const SO3Interpolator* so3interpolator,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );

    virtual ~AcquisitionDiffusoidCartesianField();

    static void checkOrInitializeDefaultParameters(
                        std::vector< double >& specificScalarParameters,
                        std::vector< std::string >& specificStringParameters );

  protected:

    Diffusoid
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    Diffusoid
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

};


}


#endif

