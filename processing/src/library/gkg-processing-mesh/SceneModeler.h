#ifndef _gkg_processing_mesh_SceneModeler_h_
#define _gkg_processing_mesh_SceneModeler_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-processing-mesh/EvolvingMeshCache.h>
#include <gkg-processing-mesh/BresenhamAlgorithm.h>
#include <map>
#include <vector>


#define DEFAULT_TIME_STEP                   1


namespace gkg
{


//
// units for:
// - time step                 -> us
//


class SceneModeler : public RCObject
{

  public:

    SceneModeler( const BoundingBox< float >& boundingBox,
                  float timeStep,
                  int32_t stepCount,
                  int32_t temporalSubSamplingCount,
                  const Vector3d< int32_t >& cacheSize,
                  bool verbose );
    virtual ~SceneModeler();

    const BoundingBox< float >& getBoundingBox() const;
    float getTimeStep() const;
    int32_t getStepCount() const;
    int32_t getTemporalSubSamplingCount() const;
    const Vector3d< int32_t >& getCacheSize() const;
    const BresenhamAlgorithm& getBresenhamAlgorithm() const;

    void getCacheVoxel( const Vector3d< float >& point,
                        Vector3d< int32_t >& voxel ) const;


    // methods to add evolvingMeshs
    void addEvolvingMesh( const std::string& name,
                          RCPointer< EvolvingMesh > evolvingMesh );
    int32_t getEvolvingMeshCount() const;

    const EvolvingMeshCache& getEvolvingMeshCache() const;
    RCPointer< EvolvingMesh > getEvolvingMesh( const std::string& name ) const;
    RCPointer< EvolvingMesh > getClosestEvolvingMesh( 
                                                 const Vector3d< float >& point,
                                                 float& distance ) const;
    bool isInsideAEvolvingMesh( const Vector3d< float >& point ) const;

    // methods to get bounding box renderings
    void getBoundingBoxOutline( const BoundingBox< float >& boundingBox,
                                Curve3dMap< float >& boundingBoxOutline ) const;

    // methods to get evolvingMesh renderings
    void getGlobalEvolvingMesh(
                      const BoundingBox< float >& boundingBox,
                      MeshMap< int32_t, float, 3U >& globalEvolvingMesh ) const;

    virtual void simulate();

  protected:

    virtual void iterate( int32_t iteration );

    BoundingBox< float > _boundingBox;
    float _timeStep;
    int32_t _stepCount;
    int32_t _temporalSubSamplingCount;
    Vector3d< int32_t > _cacheSize;
    BresenhamAlgorithm _bresenhamAlgorithm;
    bool _verbose;
    EvolvingMeshCache _evolvingMeshCache;

    std::map< std::string, RCPointer< EvolvingMesh > > _evolvingMeshes;

};


}


#endif
