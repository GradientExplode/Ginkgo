#ifndef _gkg_dmri_global_tractography_AnatomicalPriors_h_
#define _gkg_dmri_global_tractography_AnatomicalPriors_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class AnatomicalPriors
{

  public:

    AnatomicalPriors( const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters,
                      const Vector3d< int32_t >& maskSize,
                      const Vector3d< double >& maskResolution );
    AnatomicalPriors( const AnatomicalPriors& other );
    virtual ~AnatomicalPriors();

    AnatomicalPriors& operator=( const AnatomicalPriors& other );

    float getSigmoidCoefficient( float value ) const;

    bool getDistanceAndNormalToPialSurface(
                                 const Vector3d< float >& position,
                                 float& distanceToPialSurface,
                                 Vector3d< float >& normalToPialSurface ) const;

    bool getDistanceToPialSurface( const Vector3d< float >& position,
                                   float& distance ) const;

    float getOrientationDispersion( const Vector3d< float >& position ) const;
    float getSpinGlassDensity( const Vector3d< float >& position ) const; 
    float getConnectionLikelihood( const Vector3d< float >& position ) const;
    float getCurvatureThreshold( const Vector3d< float >& position ) const;
    float getMinimumDistanceToMesh() const;
    float getMaximumDistanceToMesh() const;

    float getDistanceToVentricles( const Vector3d< float >& position ) const;


    bool isFarFromPial( const Vector3d< float >& position ) const;

    bool hasPialSurface() const;

  protected:


    void readTransform3d(
                         const std::string& fileName,
                         RCPointer< Transform3d< float > >& transform3d ) const;

    float _orientationDispersionScaling;
    float _spinGlassDensity;
    float _connectionLikelihood;
    float _curvatureThreshold;
    float _minimumDistanceToMesh;
    float _maximumDistanceToMesh;
    int32_t _cacheStride;

    QuickResampler< float >* _quickResampler;


    RCPointer< MeshMap< int32_t, float, 3U > > _pialSurface;
    RCPointer< Transform3d< float > >
                      _transform3dFromPialSurfaceFrameToSpinGlassFrame;
    RCPointer< SceneModeler > _sceneModeler;
    RCPointer< EvolvingMesh > _evolvingPialSurface;
    RCPointer< Volume< uint8_t > > _enlargedPialMask;
    BoundingBox< float > _pialSurfaceBoundingBox;

    // orientation dispersion volume
    RCPointer< Volume< float > > _orientationDispersionVolume;
    RCPointer< Transform3d< float > >
                     _transform3dFromOrientationDispersionFrameToSpinGlassFrame;
    Vector3d< double > _orientationDispersionVolumeResolution;

    // spin glass density volume
    RCPointer< Volume< float > > _spinGlassDensityVolume;
    RCPointer< Transform3d< float > >
                          _transform3dFromSpinGlassDensityFrameToSpinGlassFrame;
    Vector3d< double > _spinGlassDensityVolumeResolution;

    // connection likelihood volume
    RCPointer< Volume< float > > _connectionLikelihoodVolume;
    RCPointer< Transform3d< float > >
                      _transform3dFromConnectionLikelihoodFrameToSpinGlassFrame;
    Vector3d< double > _connectionLikelihoodVolumeResolution;

    // curvature threshold volume
    RCPointer< Volume< float > > _curvatureThresholdVolume;
    RCPointer< Transform3d< float > >
                        _transform3dFromCurvatureThresholdFrameToSpinGlassFrame;
    Vector3d< double > _curvatureThresholdVolumeResolution;

    // distance to ventricles volume
    RCPointer< Volume< float > > _distanceToVentriclesVolume;
    RCPointer< Transform3d< float > >
                      _transform3dFromDistanceToVentriclesFrameToSpinGlassFrame;
    Vector3d< double > _distanceToVentriclesVolumeResolution;


};


}


#endif
