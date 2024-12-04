#ifndef _gkg_dmri_container_DiffusoidCartesianField_h_
#define _gkg_dmri_container_DiffusoidCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class DiffusoidCartesianField : public DiffusionCartesianField< Diffusoid >
{

  public:

    DiffusoidCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                             const Vector3d< double >& resolution,
                             const SiteMap< int32_t, int32_t >& siteMap,
                             const OrientationSet& outputOrientationSet,
                             const SO3Interpolator* so3interpolator );
    DiffusoidCartesianField( const SiteMap< int32_t, int32_t >& siteMap,
                             const TextureMap< Diffusoid >& items );
    virtual ~DiffusoidCartesianField();

    const OrientationSet& getOutputOrientationSet() const;
    const SO3Interpolator* getSO3Interpolator() const;

  protected:

    DiffusoidCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const OrientationSet* outputOrientationSet = 0,
                             const SO3Interpolator* so3interpolator = 0 );
    DiffusoidCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< float >& boundingBox,
                             const Vector3d< float >& superResolution,
                             const OrientationSet* outputOrientationSet = 0,
                             const SO3Interpolator* so3interpolator = 0 );
    DiffusoidCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< int32_t >& boundingBox,
                             const OrientationSet* outputOrientationSet = 0,
                             const SO3Interpolator* so3interpolator = 0 );

    void setOutputOrientationSet( const OrientationSet* outputOrientationSet );

    void getReconstructionMatrix( bool verbose );

    OrientationSet _outputOrientationSet;
    const SO3Interpolator* _so3Interpolator;
    Matrix _reconstructionMatrix;

};


}


#endif
