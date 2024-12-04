#ifndef _gkg_dmri_qspace_sampling_QSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_QSpaceSampling_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-container/MeshMap.h>
#include <list>


namespace gkg
{


class QSpaceSampling : public RCObject
{

  public:

    virtual ~QSpaceSampling();

    int32_t getCount() const;
    float getQMagnitude( int32_t rank ) const;
    float getBValue( int32_t rank ) const;
    float getDiffusionTime( int32_t rank ) const;
    // be careful: the 3D transform and the motion correction 
    // is applied to the provided orientation !
    Vector3d< float > getOrientation( int32_t rank ) const;
    Vector3d< float > getMotionRotation3d( int32_t rank ) const;

    void setMotionRotation3d( int32_t rank,
                              const Vector3d< float >& motionRotation3d );
    void addMotionRotation3d( int32_t rank,
                              const Vector3d< float >& motionRotation3d );

    const Dictionary& getGradientCharacteristics() const;

    virtual bool isCartesian() const = 0;
    virtual bool isSpherical() const = 0;
    virtual void addInformationToHeader(
                                     HeaderedObject& headeredObject ) const;

    virtual void getMeshes(
                     float sphereRadius,
                     int32_t sphereVertexCount,
                     bool withSymmetrical,
                     std::list< MeshMap< int32_t, float, 3U > >& meshes ) const;


    const Dictionary& getRawGradientCharacteristics() const;
    const RCPointer< HomogeneousTransform3d< float > >&
                                                      getRawTransform3d() const;
    const std::vector< float >& getRawBValues() const;
    const std::vector< float >& getRawDiffusionTimes() const;
    const std::vector< Vector3d< float > >& getRawMotionRotation3ds() const;
    const std::vector< Vector3d< float > >& getRawOrientations() const;

    void saveBValAndBVecFiles( const std::string& fileNameBVal,
                               const std::string& fileNameBVec ) const;
                               

  protected:

    QSpaceSampling( const HomogeneousTransform3d< float >& transform3d,
                    const Dictionary& gradientCharacteristics );

    void extractDiffusionTimes();
    void addGradientCharacteristicsToDiffusionSensitizationParameters(
                           Dictionary& diffusionSensitizationParameters ) const;

    Dictionary _gradientCharacteristics;

    RCPointer< /*Transform3d< float >*/
               HomogeneousTransform3d< float > > _transform3d;

    std::vector< float > _bValues;
    std::vector< float > _diffusionTimes;
    std::vector< Vector3d< float > > _motionRotation3ds;
    std::vector< Vector3d< float > > _orientations;

};


}


#endif
