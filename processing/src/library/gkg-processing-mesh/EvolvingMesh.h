#ifndef _gkg_processing_mesh_EvolvingMesh_h_
#define _gkg_processing_mesh_EvolvingMesh_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-mesh/PolygonCache.h>
#include <gkg-processing-mesh/VertexEvolutionFunction.h>
#include <vector>


namespace gkg
{


class SceneModeler;


class EvolvingMesh : public RCObject
{

  public:

    EvolvingMesh( RCPointer< SceneModeler > sceneModeler,
                  RCPointer< MeshMap< int32_t, float, 3U > > mesh,
                  RCPointer< VertexEvolutionFunction > vertexEvolutionFunction,
                  float radiusOfInfluence,
                  bool storeVertexTrajectories );
    virtual ~EvolvingMesh();

    RCPointer< SceneModeler > getSceneModeler() const;
    RCPointer< MeshMap< int32_t, float, 3U > > getMesh() const;
    RCPointer< VertexEvolutionFunction > getVertexEvolutionFunction() const;
    float getRadiusOfInfluence() const;
    bool storeVertexTrajectories() const;
    const PolygonCache& getPolygonCache() const;

    bool getDistance( const Vector3d< float >& point,
                      const Vector3d< int32_t >& voxel,
                      float& distance ) const;

    bool getClosestPolygonAndDistanceAndProjectionPoint(
                                     const Vector3d< float >& point,
                                     Polygon< 3U >& polygon,
                                     float& distance,
                                     Vector3d< float >& projectionPoint ) const;
    bool getFastClosestPolygonAndDistanceAndProjectionPoint(
                                     const Vector3d< float >& point,
                                     const Vector3d< int32_t >& voxel,
                                     Polygon< 3U >& polygon,
                                     float& distance,
                                     Vector3d< float >& projectionPoint ) const;
    bool getFastClosestPolygonAndDistanceAndProjectionPointAndNormal(
                                     const Vector3d< float >& point,
                                     const Vector3d< int32_t >& voxel,
                                     Polygon< 3U >& polygon,
                                     float& distance,
                                     Vector3d< float >& projectionPoint,
                                     Vector3d< float >& normal ) const;
    bool getFastDistanceAndNormal( const Vector3d< float >& point,
                                   const Vector3d< int32_t >& voxel,
                                   float& distance,
                                   Vector3d< float >& normal ) const;

    void getPartialTriangleAreaWeighting(
                                       const Polygon< 3U >& polygon,
                                       const Vector3d< float >& projectionPoint,
                                       float& vertexWeight1,
                                       float& vertexWeight2,
                                       float& vertexWeight3 ) const;


    void evaluateCroppedMesh( const BoundingBox< float >& boundingBox,
                               int32_t& croppedVertexCount,
                               int32_t& croppedPolygonCount ) const;
    RCPointer< MeshMap< int32_t, float, 3U > > 
      getCroppedMesh( const BoundingBox< float >& boundingBox ) const;
    RCPointer< MeshMap< int32_t, float, 3U > > getEvolvingMesh() const;

    void move( int32_t iteration );

    const Vector3d< float >& getCurrentVertex( int32_t index ) const;
    const Curve3d< float >& getVertexTrajectory( int32_t index ) const;

  protected:

    RCPointer< SceneModeler > _sceneModeler;
    RCPointer< MeshMap< int32_t, float, 3U > > _mesh;
    std::list< Vector3d< float > > _normals;
    RCPointer< VertexEvolutionFunction > _vertexEvolutionFunction;
    float _radiusOfInfluence;
    bool _storeVertexTrajectories;
    PolygonCache _polygonCache;
    PolygonNormalFunctor< int32_t, float, 3U > _polygonNormalFunctor;

    std::vector< const Vector3d< float >* > _initialVertices;
    std::vector< Vector3d< float > > _currentVertices;
    std::vector< Curve3d< float > > _vertexTrajectories;

};


}


#endif

