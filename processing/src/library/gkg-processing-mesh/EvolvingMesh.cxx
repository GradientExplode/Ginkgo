#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


gkg::EvolvingMesh::EvolvingMesh( 
                      gkg::RCPointer< gkg::SceneModeler > sceneModeler,
                      gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > mesh,
                      gkg::RCPointer< gkg::VertexEvolutionFunction > 
                        vertexEvolutionFunction,
                      float radiusOfInfluence,
                      bool storeVertexTrajectories )
              : gkg::RCObject(),
                _sceneModeler( sceneModeler ),
                _mesh( mesh ),
                _normals( mesh->getNormals( 0 ) ),
                _vertexEvolutionFunction( vertexEvolutionFunction ),
		_radiusOfInfluence( radiusOfInfluence ),
                _storeVertexTrajectories( storeVertexTrajectories ),
                _polygonCache( gkg::RCPointer< gkg::EvolvingMesh >( this ),
                               sceneModeler->getCacheSize(),
                               radiusOfInfluence )
{

  try
  {

    // creating the rank/site lut for the mesh
    gkg::RankSiteLut< int32_t, float > rankSiteLut( _mesh->vertices );

    // collecting the list of initial vertices
    const std::list< gkg::Vector3d< float > >& 
      vertices = _mesh->vertices.getSites( 0 );

    _initialVertices.resize( vertices.size() );
    _currentVertices.resize( vertices.size() );
    _vertexTrajectories.resize( vertices.size() );

    int32_t index = 0;
    std::list< gkg::Vector3d< float > >::const_iterator
      v = vertices.begin(),
      ve = vertices.end();
    while ( v != ve )
    {

      index = rankSiteLut.getIndex( 0, *v );
      _initialVertices[ index ] = &( *v );
      _currentVertices[ index ] = *v;

      if ( _storeVertexTrajectories )
      {

        _vertexTrajectories[ index ].addPoint( *v );

      }
      ++ v;

    }

  }
  GKG_CATCH( "gkg::EvolvingMesh::EvolvingMesh( "
             "gkg::RCPointer< gkg::SceneModeler > sceneModeler, "
             "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > mesh, "
             "gkg::RCPointer< gkg::VertexEvolutionFunction > "
             "  vertexEvolutionFunction, "
             "float radiusOfInfluence, "
             "bool storeVertexTrajectories )" );

}


gkg::EvolvingMesh::~EvolvingMesh()
{
}

    
gkg::RCPointer< gkg::SceneModeler > gkg::EvolvingMesh::getSceneModeler() const
{

  try
  {

    return _sceneModeler;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::SceneModeler > "
             "gkg::EvolvingMesh::getSceneModeler() const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::EvolvingMesh::getMesh() const
{

  try
  {

    return _mesh;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::EvolvingMesh::getMesh() const" );

}


gkg::RCPointer< gkg::VertexEvolutionFunction > 
gkg::EvolvingMesh::getVertexEvolutionFunction() const
{

  try
  {

    return _vertexEvolutionFunction;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::VertexEvolutionFunction > "
             "gkg::EvolvingMesh::getVertexEvolutionFunction() const" );

}


float gkg::EvolvingMesh::getRadiusOfInfluence() const
{

  try
  {

    return _radiusOfInfluence;

  }
  GKG_CATCH( "float gkg::EvolvingMesh::getRadiusOfInfluence() const" );

}


bool gkg::EvolvingMesh::storeVertexTrajectories() const
{

  try
  {

    return _storeVertexTrajectories;

  }
  GKG_CATCH( "bool gkg::EvolvingMesh::storeVertexTrajectories() const" );

}

const gkg::PolygonCache& gkg::EvolvingMesh::getPolygonCache() const
{

  try
  {

    return _polygonCache;

  }
  GKG_CATCH( "const gkg::PolygonCache& "
             "gkg::EvolvingMesh::getPolygonCache() const" );

}


bool gkg::EvolvingMesh::getDistance(
                                   const gkg::Vector3d< float >& point,
                                   const gkg::Vector3d< int32_t >& voxel,
                                   float& distance ) const
{

  try
  {

    bool distanceIsValid = false;
    distance = 1e38;

    // collecting the set of polygons corresponding to the current
    // membrane within the current voxel
    std::set< const gkg::Polygon< 3U >* >*
      polygons = _polygonCache.getPolygons( voxel );

    if ( polygons )
    {

      distanceIsValid = true;

      // looping over the polygons
      gkg::Vector3d< float > vertex1, vertex2, vertex3;
      std::set< const gkg::Polygon< 3U >* >::const_iterator
        p = polygons->begin(),
        pe = polygons->end();

      float currentDistance = 0.0;        
      while ( p != pe )
      {

        // obtaining the current vertices of the polygon
        vertex1 = getCurrentVertex( ( *p )->indices[ 0 ] );
        vertex2 = getCurrentVertex( ( *p )->indices[ 1 ] );
        vertex3 = getCurrentVertex( ( *p )->indices[ 2 ] );
  
        // getting the point to the triangle distance
        currentDistance = gkg::getPointToTriangleDistance( point,
                                                           vertex1,
                                                           vertex2,
                                                           vertex3 );
        if ( currentDistance < distance )
        {

          distance = currentDistance;

        }
      
        ++ p;

      }

    }    
    return distanceIsValid;

  }
  GKG_CATCH( "bool gkg::EvolvingMesh::getDistance( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "float& distance ) const" );

}


bool gkg::EvolvingMesh::getClosestPolygonAndDistanceAndProjectionPoint(
                                 const gkg::Vector3d< float >& point,
                                 gkg::Polygon< 3U >& polygon,
                                 float& distance,
                                 gkg::Vector3d< float >& projectionPoint ) const
{

  try
  {

    bool distanceIsValid = false;
    distance = 1e38;

    // collecting the cache voxel corresponding to the point
    gkg::Vector3d< int32_t > voxel;
    _sceneModeler->getCacheVoxel( point, voxel );

    // collecting the set of polygons corresponding to the current
    // membrane within the current voxel
    std::set< const gkg::Polygon< 3U >* >*
      polygons = _polygonCache.getPolygons( voxel );

    if ( polygons )
    {

      distanceIsValid = true;

      // looping over the polygons
      gkg::Vector3d< float > vertex1, vertex2, vertex3;
      std::set< const gkg::Polygon< 3U >* >::const_iterator
        p = polygons->begin(),
        pe = polygons->end();

      float currentDistance = 0.0;
      gkg::Vector3d< float > currentProjectionPoint;
      while ( p != pe )
      {

        // obtaining the current vertices of the polygon
        vertex1 = getCurrentVertex( ( *p )->indices[ 0 ] );
        vertex2 = getCurrentVertex( ( *p )->indices[ 1 ] );
        vertex3 = getCurrentVertex( ( *p )->indices[ 2 ] );
  
        // getting the point to the triangle distance
        currentDistance = gkg::getPointToTriangleDistanceAndProjectionPoint(
                                                       point,
                                                       vertex1,
                                                       vertex2,
                                                       vertex3,
                                                       currentProjectionPoint );
        if ( currentDistance < distance )
        {

          polygon = **p;
          distance = currentDistance;
          projectionPoint = currentProjectionPoint;

        }
      
        ++ p;

      }

    }
    else
    {

      polygon.indices[ 0 ] = 0;
      polygon.indices[ 1 ] = 0;
      polygon.indices[ 2 ] = 0;
      distance = 1e38;
      projectionPoint.x = 0.0;
      projectionPoint.y = 0.0;
      projectionPoint.z = 0.0;

    }

    return distanceIsValid;

  }
  GKG_CATCH( "bool "
             "gkg::EvolvingMesh::"
             "getClosestPolygonAndDistanceAndProjectionPoint( "
             "const gkg::Vector3d< float >& point, "
             "gkg::Polygon< 3U >& polygon, "
             "float& distance, "
             "gkg::Vector3d< float >& projectionPoint ) const" );

}


bool gkg::EvolvingMesh::getFastClosestPolygonAndDistanceAndProjectionPoint(
                                 const gkg::Vector3d< float >& point,
                                 const gkg::Vector3d< int32_t >& voxel,
                                 gkg::Polygon< 3U >& polygon,
                                 float& distance,
                                 gkg::Vector3d< float >& projectionPoint ) const
{

  try
  {

    bool distanceIsValid = false;
    distance = 1e38;

    // collecting the set of polygons corresponding to the current
    // membrane within the current voxel
    std::set< const gkg::Polygon< 3U >* >*
      polygons = _polygonCache.getPolygons( voxel );

    if ( polygons )
    {

      distanceIsValid = true;

      // looping over the polygons
      gkg::Vector3d< float > vertex1, vertex2, vertex3;
      std::set< const gkg::Polygon< 3U >* >::const_iterator
        p = polygons->begin(),
        pe = polygons->end();

      float currentDistance = 0.0;
      gkg::Vector3d< float > currentProjectionPoint;
      while ( p != pe )
      {

        // getting the point to the triangle distance
        currentDistance = gkg::getPointToTriangleDistanceAndProjectionPoint(
                                       point,
                                       getCurrentVertex( ( *p )->indices[ 0 ] ),
                                       getCurrentVertex( ( *p )->indices[ 1 ] ),
                                       getCurrentVertex( ( *p )->indices[ 2 ] ),
                                       currentProjectionPoint );
        if ( currentDistance < distance )
        {

          polygon = **p;
          distance = currentDistance;
          projectionPoint = currentProjectionPoint;

        }
      
        ++ p;

      }

    }

    return distanceIsValid;

  }
  GKG_CATCH( "bool "
             "gkg::EvolvingMesh::"
             "getFastClosestPolygonAndDistanceAndProjectionPoint( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::Polygon< 3U >& polygon, "
             "float& distance, "
             "gkg::Vector3d< float >& projectionPoint ) const" );

}


bool 
gkg::EvolvingMesh::getFastClosestPolygonAndDistanceAndProjectionPointAndNormal(
                                 const gkg::Vector3d< float >& point,
                                 const gkg::Vector3d< int32_t >& voxel,
                                 gkg::Polygon< 3U >& polygon,
                                 float& distance,
                                 gkg::Vector3d< float >& projectionPoint,
                                 gkg::Vector3d< float >& normal ) const
{

  try
  {

    bool distanceIsValid = false;
    distance = 1e38;

    // collecting the set of polygons corresponding to the current
    // membrane within the current voxel
    std::set< const gkg::Polygon< 3U >* >*
      polygons = _polygonCache.getPolygons( voxel );

    if ( polygons )
    {

      distanceIsValid = true;

      // looping over the polygons
      std::set< const gkg::Polygon< 3U >* >::const_iterator
        p = polygons->begin(),
        pe = polygons->end();

      float currentDistance = 0.0;
      gkg::Vector3d< float > currentProjectionPoint;
      gkg::Vector3d< float > v1;
      gkg::Vector3d< float > v2;
      gkg::Vector3d< float > v3;
      while ( p != pe )
      {

        v1 = getCurrentVertex( ( *p )->indices[ 0 ] );
        v2 = getCurrentVertex( ( *p )->indices[ 1 ] );
        v3 = getCurrentVertex( ( *p )->indices[ 2 ] );

        // getting the point to the triangle distance
        currentDistance = gkg::getPointToTriangleDistanceAndProjectionPoint(
                                                       point,
                                                       v1,
                                                       v2,
                                                       v3,
                                                       currentProjectionPoint );
        if ( currentDistance < distance )
        {

          polygon = **p;
          distance = currentDistance;
          projectionPoint = currentProjectionPoint;
          normal = ( v2 - v1 ).cross( v3 - v1 );
          normal.normalize();

        }
      
        ++ p;

      }

    }

    return distanceIsValid;

  }
  GKG_CATCH( "bool "
             "gkg::EvolvingMesh::"
             "getFastClosestPolygonAndDistanceAndProjectionPoint( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::Polygon< 3U >& polygon, "
             "float& distance, "
             "gkg::Vector3d< float >& projectionPoint, "
             "gkg::Vector3d< float >& normal ) const" );

}


bool gkg::EvolvingMesh::getFastDistanceAndNormal(
                                          const gkg::Vector3d< float >& point,
                                          const gkg::Vector3d< int32_t >& voxel,
                                          float& distance,
                                          gkg::Vector3d< float >& normal ) const
{

  try
  {

    bool distanceIsValid = false;
    distance = 1e38;

    // collecting the set of polygons corresponding to the current
    // membrane within the current voxel
    std::set< const gkg::Polygon< 3U >* >*
      polygons = _polygonCache.getPolygons( voxel );

    if ( polygons )
    {

      distanceIsValid = true;

      // looping over the polygons
      std::set< const gkg::Polygon< 3U >* >::const_iterator
        p = polygons->begin(),
        pe = polygons->end();

      float currentDistance = 0.0;
      gkg::Vector3d< float > v1;
      gkg::Vector3d< float > v2;
      gkg::Vector3d< float > v3;
      while ( p != pe )
      {

        v1 = getCurrentVertex( ( *p )->indices[ 0 ] );
        v2 = getCurrentVertex( ( *p )->indices[ 1 ] );
        v3 = getCurrentVertex( ( *p )->indices[ 2 ] );

        // getting the point to the triangle distance
        currentDistance = gkg::getPointToTriangleDistance( point, v1, v2, v3 );

        if ( currentDistance < distance )
        {

          distance = currentDistance;
          normal = ( v2 - v1 ).cross( v3 - v1 );
          normal.normalize();

        }
      
        ++ p;

      }

    }

    return distanceIsValid;

  }
  GKG_CATCH( "bool gkg::EvolvingMesh::getFastDistanceAndNormal( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "float& distance, gkg::Vector3d< float >& normal ) const" );

}


void gkg::EvolvingMesh::getPartialTriangleAreaWeighting(
                                       const gkg::Polygon< 3U >& polygon,
                                       const Vector3d< float >& projectionPoint,
                                       float& vertexWeight1,
                                       float& vertexWeight2,
                                       float& vertexWeight3 ) const
{

  try
  {

    float area = gkg::getPartialTriangleAreas(
                              projectionPoint,
                              this->getCurrentVertex( polygon.indices[ 0 ] ),
                              this->getCurrentVertex( polygon.indices[ 1 ] ),
                              this->getCurrentVertex( polygon.indices[ 2 ] ),
                              vertexWeight1,
                              vertexWeight2,
                              vertexWeight3 );

    vertexWeight1 /= area;
    vertexWeight2 /= area;
    vertexWeight3 /= area;

  }
  GKG_CATCH( "void gkg::EvolvingMesh::getPartialWeighting( "
             "const gkg::Polygon< 3U >& polygon, "
             "const Vector3d< float >& projectionPoint, "
             "float& vertexWeight1, "
             "float& vertexWeight2, "
             "float& vertexWeight3 ) const" );


}


void gkg::EvolvingMesh::evaluateCroppedMesh(
                                   const gkg::BoundingBox< float >& boundingBox,
                                   int32_t& croppedVertexCount,
                                   int32_t& croppedPolygonCount ) const
{

  try
  {

    // initializing output counts
    croppedVertexCount = 0;
    croppedPolygonCount = 0;

    // collecting the polygons of the mesh
    const std::list< gkg::Polygon< 3U > >& 
      polygonList = _mesh->polygons.getPolygons( 0 );

    // creating a rank/Site Lut for the vertices of the mesh
    gkg::RankSiteLut< int32_t, float > oldRankSiteLut( _mesh->vertices );

    // loop over each polygon in the mesh and building the list of the kept
    // polygons
    std::set< gkg::Vector3d< float >,
              gkg::Vector3dCompare< float > > keptVertices;

    gkg::Vector3d< float > vertex1, vertex2, vertex3;
    std::list< gkg::Polygon< 3U > >::const_iterator
      p = polygonList.begin(),
      pe = polygonList.end();
    while ( p != pe )
    {

      // using rank/site lut to get the site of a given polygon
      vertex1 = oldRankSiteLut.getRankSite( p->indices[ 0 ] ).site;
      vertex2 = oldRankSiteLut.getRankSite( p->indices[ 1 ] ).site;
      vertex3 = oldRankSiteLut.getRankSite( p->indices[ 2 ] ).site;

        // if site contained in the bounding box
      if ( boundingBox.contains( vertex1 ) ||
           boundingBox.contains( vertex2 ) ||
           boundingBox.contains( vertex3 ) )
      {

          // we need to keep that polygon and to keep the associated
          // vertices
          ++ croppedPolygonCount;
          keptVertices.insert( vertex1 );
          keptVertices.insert( vertex2 );
          keptVertices.insert( vertex3 );

      }
      ++ p;

    }

    croppedVertexCount = ( int32_t )keptVertices.size();
  
  }
  GKG_CATCH( "void gkg::EvolvingMesh::evaluateCroppedMesh( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "int32_t& croppedVertexCount, "
             "int32_t& croppedPolygonCount ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::EvolvingMesh::getCroppedMesh(
                            const gkg::BoundingBox< float >& boundingBox ) const
{

  try
  {
  
    // collecting the polygons of the mesh
    const std::list< gkg::Polygon< 3U > >& 
      polygonList = _mesh->polygons.getPolygons( 0 );

    // creating a rank/Site Lut for the vertices of the mesh
    gkg::RankSiteLut< int32_t, float > oldRankSiteLut( _mesh->vertices );

    // loop over each polygon in the mesh and building the list of kept
    // polygons
    std::list< gkg::Polygon< 3U > > keptPolygons;
    std::set< gkg::Vector3d< float >,
              gkg::Vector3dCompare< float > > keptVertices;
    std::map< uint32_t, uint32_t > oldIndexToNewIndexLut;

    gkg::Vector3d< float > vertex1, vertex2, vertex3;
    std::list< gkg::Polygon< 3U > >::const_iterator
      p = polygonList.begin(),
      pe = polygonList.end();
    while ( p != pe )
    {

      // using rank/site lut to get the site of a given polygon
      vertex1 = oldRankSiteLut.getRankSite( p->indices[ 0 ] ).site;
      vertex2 = oldRankSiteLut.getRankSite( p->indices[ 1 ] ).site;
      vertex3 = oldRankSiteLut.getRankSite( p->indices[ 2 ] ).site;

        // if site contained in the bounding box
      if ( boundingBox.contains( vertex1 ) ||
           boundingBox.contains( vertex2 ) ||
           boundingBox.contains( vertex3 ) )
      {

        // we need to keep that polygon and to keep the associated
        // vertices
        keptVertices.insert( vertex1 );
        keptVertices.insert( vertex2 );
        keptVertices.insert( vertex3 );
        keptPolygons.push_back( *p );

      }
      ++ p;

    }

    // creating old to new vertex index map
    std::set< gkg::Vector3d< float >,
              gkg::Vector3dCompare< float > >::const_iterator
      s = keptVertices.begin(),
      se = keptVertices.end();
    uint32_t index = 0;
    while ( s != se )
    {

      oldIndexToNewIndexLut[ ( uint32_t )oldRankSiteLut.getIndex( 0, *s ) ] =
        ( uint32_t )index;
      ++ index;
      ++ s;

    }

    // updating the kept polygon indices
    uint32_t v = 0;
    std::list< gkg::Polygon< 3U > >::iterator
      kp = keptPolygons.begin(),
      kpe = keptPolygons.end();
    while ( kp != kpe )
    {

      for ( v = 0U; v < 3U; v++ )
      {

        kp->indices[ v ] = oldIndexToNewIndexLut[ kp->indices[ v ] ];

      }
      ++ kp;

    }

    // creating the new cropped mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      croppedMeshMap( new gkg::MeshMap< int32_t, float, 3U > );
    croppedMeshMap->vertices.addSites( 0, keptVertices );
    croppedMeshMap->polygons.addPolygons( 
                                        croppedMeshMap->vertices.getIndex( 0 ),
                                        keptPolygons );

    return croppedMeshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::EvolvingMesh::getCroppedMesh( "
             "const gkg::BoundingBox< float >& boundingBox ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::EvolvingMesh::getEvolvingMesh() const
{

  try
  {

    // allocating output mesh map
    RCPointer< MeshMap< int32_t, float, 3U > >
      evolvingMeshMap( new gkg::MeshMap< int32_t, float, 3U > );

    // adding vertices to the evolvingMeshMap for all the rank(s)
    int32_t rank = _vertexTrajectories[ 0 ].getPointCount();
    int32_t r = 0;
    std::vector< gkg::Curve3d< float > >::const_iterator
      t = _vertexTrajectories.begin(),
      te = _vertexTrajectories.end();
    while ( t != te )
    {

      for ( r = 0; r < rank; r++ )
      {

        evolvingMeshMap->vertices.addSite( r, t->getPoint( r ) );

      }
      ++ t;

    }

    // adding polygons to the evolvingMeshMap
    const std::list< gkg::Polygon< 3U > >& 
      polygonList = _mesh->polygons.getPolygons( 0 );
    for ( r = 0; r < rank; r++ )
    {

      evolvingMeshMap->polygons.addPolygons( r, polygonList );

    }

    return evolvingMeshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::EvolvingMesh::getEvolvingMesh() const" );

}


void gkg::EvolvingMesh::move( int32_t iteration )
{

  try
  {

    // updating the vertex evolution function
    _vertexEvolutionFunction->setStep( iteration );

    // modifying the vertices using the normals and the vertex evolution
    // function, and storing them
    std::vector< gkg::Vector3d< float > >::iterator
      cv = _currentVertices.begin(),
      cve = _currentVertices.end();
    std::vector< const gkg::Vector3d< float >* >::const_iterator
      iv = _initialVertices.begin();
    std::list< gkg::Vector3d< float > >::const_iterator
      n = _normals.begin();
    std::vector< gkg::Curve3d< float > >::iterator
      t = _vertexTrajectories.begin();
    while ( cv != cve )
    {

      // initializing the current vertex from the initial vertex
      *cv = **iv;

      // modifying the current vertex along the normal
      _vertexEvolutionFunction->modifyVertex( *cv, *n );

      // if required, storing the result in a dedicated map of trajectories
      if ( ( _storeVertexTrajectories ) &&
           ( !( iteration % _sceneModeler->getTemporalSubSamplingCount() ) ||
             ( iteration == ( _sceneModeler->getStepCount() - 1 ) ) ) )
      {

        t->addPoint( *cv );

      }
      ++ cv;
      ++ iv;
      ++ n;
      ++ t;

    }

  }
  GKG_CATCH( "void gkg::EvolvingMesh::membraneEvolution( int32_t iteration )" );

}


const gkg::Vector3d< float >& 
gkg::EvolvingMesh::getCurrentVertex( int32_t index ) const
{

  try
  {

    return _currentVertices[ index ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::EvolvingMesh::getCurrentVertex( int32_t index ) const" );

}


const gkg::Curve3d< float >&
gkg::EvolvingMesh::getVertexTrajectory( int32_t index ) const
{

  try
  {

    return _vertexTrajectories[ index ];

  }
  GKG_CATCH( "const gkg::Curve3d< float >& "
             "gkg::EvolvingMesh::getVertexTrajectory( int32_t index ) const" );

}

