#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/IntersectionSet.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>
#include <set>
#include <iostream>
#include <iomanip>
#include <cmath>


gkg::SceneModeler::SceneModeler( const gkg::BoundingBox< float >& boundingBox,
                                 float timeStep,
                                 int32_t stepCount,
                                 int32_t temporalSubSamplingCount,
                                 const gkg::Vector3d< int32_t >& cacheSize,
                                 bool verbose )
                  : gkg::RCObject(),
                    _boundingBox( boundingBox ),
                    _timeStep( timeStep ),
                    _stepCount( stepCount ),
                    _temporalSubSamplingCount( temporalSubSamplingCount ),
                    _cacheSize( cacheSize ),
                    _bresenhamAlgorithm( boundingBox, cacheSize ),
                    _verbose( verbose ),
                    _evolvingMeshCache(
                                    gkg::RCPointer< gkg::SceneModeler >( this ),
                                    cacheSize )
{
}


gkg::SceneModeler::~SceneModeler()
{
}


const gkg::BoundingBox< float >& gkg::SceneModeler::getBoundingBox() const
{

  try
  {

    return _boundingBox;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::SceneModeler::getBoundingBox() const" );

}


float gkg::SceneModeler::getTimeStep() const
{

  try
  {

    return _timeStep;

  }
  GKG_CATCH( "float gkg::SceneModeler::getTimeStep() const" );

}


int32_t gkg::SceneModeler::getStepCount() const
{

  try
  {

    return _stepCount;

  }
  GKG_CATCH( "int32_t gkg::SceneModeler::getStepCount() const" );

}


int32_t gkg::SceneModeler::getTemporalSubSamplingCount() const
{

  try
  {

    return _temporalSubSamplingCount;

  }
  GKG_CATCH( "int32_t gkg::SceneModeler::getTemporalSubSamplingCount() const" );

}


const gkg::Vector3d< int32_t >& gkg::SceneModeler::getCacheSize() const
{

  try
  {

    return _cacheSize;

  }
  GKG_CATCH( "const gkg::Vector3d< int32_t >& "
             "gkg::SceneModeler::getCacheSize() const" );

}


const gkg::BresenhamAlgorithm& gkg::SceneModeler::getBresenhamAlgorithm() const
{

  try
  {

    return _bresenhamAlgorithm;

  }
  GKG_CATCH( "const gkg::BresenhamAlgorithm& "
             "gkg::SceneModeler::getBresenhamAlgorithm() cons" );

}


void gkg::SceneModeler::getCacheVoxel( const gkg::Vector3d< float >& point,
                                       gkg::Vector3d< int32_t >& voxel ) const
{

  try
  {

    _bresenhamAlgorithm.getCacheVoxel( point, voxel );

  }
  GKG_CATCH( "void gkg::SceneModeler::getCacheVoxel( "
             "const gkg::Vector3d< float >& point, "
             "gkg::Vector3d< int32_t >& voxel ) const" );

}


void gkg::SceneModeler::addEvolvingMesh(
                              const std::string& name,
                              gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh )
{

  try
  {

    if ( _evolvingMeshes.find( name ) != _evolvingMeshes.end() )
    {

      throw std::runtime_error( "evolvingMesh name already present" );

    }
    _evolvingMeshes[ name ] = evolvingMesh;

    // updating the evolving mesh cache
    _evolvingMeshCache.update( evolvingMesh );

  }
  GKG_CATCH( "void gkg::SceneModeler::addEvolvingMesh( "
             "const std::string& name, "
             "gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh )" );

}


int32_t gkg::SceneModeler::getEvolvingMeshCount() const
{

  try
  {
  
    return ( int32_t )_evolvingMeshes.size();
  
  }
  GKG_CATCH( "int32_t gkg::SceneModeler::getEvolvingMeshCount() const" );

}


gkg::RCPointer< gkg::EvolvingMesh > 
gkg::SceneModeler::getEvolvingMesh( const std::string& name ) const
{

  try
  {

    gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh;
    std::map< std::string, RCPointer< EvolvingMesh > >::const_iterator
      e = _evolvingMeshes.find( name );

    if ( e != _evolvingMeshes.end() )
    {

      evolvingMesh = e->second;

    }

    return evolvingMesh;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::EvolvingMesh > "
             "gkg::SceneModeler::getEvolvingMesh( "
             "const std::string& name ) const" );

}


const gkg::EvolvingMeshCache& gkg::SceneModeler::getEvolvingMeshCache() const
{

  try
  {

    return _evolvingMeshCache;

  }
  GKG_CATCH( "const gkg::EvolvingMeshCache& "
             "gkg::SceneModeler::getEvolvingMeshCache() const" );

}


gkg::RCPointer< gkg::EvolvingMesh > 
gkg::SceneModeler::getClosestEvolvingMesh( const gkg::Vector3d< float >& point,
                                           float& distance ) const
{

  try
  {

    // allocating output; by default it is empty
    gkg::RCPointer< gkg::EvolvingMesh > closestEvolvingMesh;

    if ( !_evolvingMeshes.empty() )
    {

      // getting the voxel of the point
      gkg::Vector3d< int32_t > voxel;
      _bresenhamAlgorithm.getCacheVoxel( point, voxel );

      // increasing the stride until the closest evolving mesh is found
      gkg::Vector3d< int32_t > currentVoxel;
      float currentDistance = 0.0;
      int32_t x, y, z;
      int32_t stride = 1;
      while ( closestEvolvingMesh.isNull() )
      {

        for ( x = -stride; x <= stride; x ++ )
        {

          currentVoxel.x = voxel.x + x;
          for ( y = -stride; y <= stride; y++ )
          {

            currentVoxel.y = voxel.y + y;
            for ( z = -stride; z <= stride; z++ )
            {

              currentVoxel.z = voxel.z + z;
              if ( ( currentVoxel.x >= 0 ) &&
                   ( currentVoxel.x <= ( _cacheSize.x - 1 ) ) &&
                   ( currentVoxel.y >= 0 ) &&
                   ( currentVoxel.y <= ( _cacheSize.y - 1 ) ) &&
                   ( currentVoxel.z >= 0 ) &&
                   ( currentVoxel.z <= ( _cacheSize.z - 1 ) ) )
              {

                // collecting the set of evolvingMesh crossing this voxel
                std::set< gkg::RCPointer< gkg::EvolvingMesh > >*
                  evolvingMeshes = _evolvingMeshCache.getEvolvingMeshes(
                                                                 currentVoxel );

                // if the set of evolvingMesh is not empty
                if ( evolvingMeshes )
                {

                  // looping over the evolvingMeshes
                  std::set< gkg::RCPointer<gkg::EvolvingMesh > >::const_iterator
                    m = evolvingMeshes->begin(),
                    me = evolvingMeshes->end();
                  while ( m != me )
                  {

                    ( *m )->getDistance( point,
                                         currentVoxel,
                                         currentDistance );
                    if ( currentDistance < distance )
                    {

                      distance = currentDistance;
                      closestEvolvingMesh = *m;

                    }
                    ++ m;

                  }

                }

              }

            }

          }

        }
        ++ stride;

      }

    }

    return closestEvolvingMesh;

  }
  GKG_CATCH( "gkg::RCPointer< EvolvingMesh > " 
             "gkg::SceneModeler::getClosestEvolvingMesh( "
             "const gkg::Vector3d< float >& point, "
             "float& distance ) const" );

}


bool gkg::SceneModeler::isInsideAEvolvingMesh(
                                     const gkg::Vector3d< float >& point ) const
{

  try
  {

    gkg::Vector3d< float > projectionPoint( point );

    ////// casting a ray in +x or -x direction

    // getting the projection point on the bounding box
    float upperX = _boundingBox.getUpperX();
    float lowerX = _boundingBox.getLowerX();
    if ( ( upperX - point.x ) < ( point.x - lowerX ) )
    {

      projectionPoint.x = upperX;

    }
    else
    {

      projectionPoint.x = lowerX;

    }

    // collecting intersections
    gkg::IntersectionSet intersectionSetAlongX( *this ,
                                                point,
                                                projectionPoint );
    int32_t intersectionCountAlongX = intersectionSetAlongX.getCount();

    ////// casting a ray in +y or -y direction

    // getting the projection point on the bounding box
    projectionPoint.x = point.x;
    float upperY = _boundingBox.getUpperY();
    float lowerY = _boundingBox.getLowerY();
    if ( ( upperY - point.y ) < ( point.y - lowerY ) )
    {

      projectionPoint.y = upperY;

    }
    else
    {

      projectionPoint.y = lowerY;

    }
    gkg::IntersectionSet intersectionSetAlongY( *this,
                                                point,
                                                projectionPoint );
    int32_t intersectionCountAlongY = intersectionSetAlongY.getCount();

    // checking the intersection count
    if ( ( intersectionCountAlongX % 2 ) &&
         ( intersectionCountAlongY % 2 ) )
    {

      return true;

    }
    else
    {

      return false;

    }

  }
  GKG_CATCH( "bool gkg::SceneModeler::isInsideAEvolvingMesh( "
             "const gkg::Vector3d< float >& point ) const" );

}


void gkg::SceneModeler::getBoundingBoxOutline(
                            const gkg::BoundingBox< float >& boundingBox,
                            gkg::Curve3dMap< float >& boundingBoxOutline ) const
{

  try
  {

    // allocating 3d curves
    std::vector< gkg::Curve3d< float > > curve3ds( 6 );

    // collecting the lower and upper points of the bounding box
    gkg::Vector3d< float > lowerPoint( boundingBox.getLowerX(),
                                       boundingBox.getLowerY(),
                                       boundingBox.getLowerZ() );
    gkg::Vector3d< float > upperPoint( boundingBox.getUpperX(),
                                       boundingBox.getUpperY(),
                                       boundingBox.getUpperZ() );

    // allocating a vector of vertices for the lower and upper planes
    std::vector< gkg::Vector3d< float > > plane( 5 );

    // processing the lower plane
    plane[ 0 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         lowerPoint.y,
                                         lowerPoint.z );
    plane[ 1 ] = gkg::Vector3d< float >( upperPoint.x,
                                         lowerPoint.y,
                                         lowerPoint.z );
    plane[ 2 ] = gkg::Vector3d< float >( upperPoint.x,
                                         upperPoint.y,
                                         lowerPoint.z );
    plane[ 3 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         upperPoint.y,
                                         lowerPoint.z );
    plane[ 4 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         lowerPoint.y,
                                         lowerPoint.z );
    curve3ds[ 0 ] = gkg::Curve3d< float >( plane );

    // processing the upper plane
    plane[ 0 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         lowerPoint.y,
                                         upperPoint.z );
    plane[ 1 ] = gkg::Vector3d< float >( upperPoint.x,
                                         lowerPoint.y,
                                         upperPoint.z );
    plane[ 2 ] = gkg::Vector3d< float >( upperPoint.x,
                                         upperPoint.y,
                                         upperPoint.z );
    plane[ 3 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         upperPoint.y,
                                         upperPoint.z );
    plane[ 4 ] = gkg::Vector3d< float >( lowerPoint.x,
                                         lowerPoint.y,
                                         upperPoint.z );
    curve3ds[ 1 ] = gkg::Curve3d< float >( plane );

    // allocating a vector for the lower and upper segments
    std::vector< gkg::Vector3d< float > > segment( 2 );

    // collecting the lower vertices to the upper vertices
    // segment 1
    segment[ 0 ] = gkg::Vector3d< float >( lowerPoint.x,
                                           lowerPoint.y,
                                           lowerPoint.z );
    segment[ 1 ] = gkg::Vector3d< float >( lowerPoint.x,
                                           lowerPoint.y,
                                           upperPoint.z );
    curve3ds[ 2 ] = gkg::Curve3d< float >( segment );

    // segment 2
    segment[ 0 ] = gkg::Vector3d< float >( lowerPoint.x,
                                           upperPoint.y,
                                           lowerPoint.z );
    segment[ 1 ] = gkg::Vector3d< float >( lowerPoint.x,
                                           upperPoint.y,
                                           upperPoint.z );
    curve3ds[ 3 ] = gkg::Curve3d< float >( segment );

    // segment 3
    segment[ 0 ] = gkg::Vector3d< float >( upperPoint.x,
                                           lowerPoint.y,
                                           lowerPoint.z );
    segment[ 1 ] = gkg::Vector3d< float >( upperPoint.x,
                                           lowerPoint.y,
                                           upperPoint.z );
    curve3ds[ 4 ] = gkg::Curve3d< float >( segment );

    // segment 4
    segment[ 0 ] = gkg::Vector3d< float >( upperPoint.x,
                                           upperPoint.y,
                                           lowerPoint.z );
    segment[ 1 ] = gkg::Vector3d< float >( upperPoint.x,
                                           upperPoint.y,
                                           upperPoint.z );
    curve3ds[ 5 ] = gkg::Curve3d< float >( segment );

    // connecting the vertices
    boundingBoxOutline.addCurve3ds( curve3ds );

  }
  GKG_CATCH( "void gkg::SceneModeler::getBoundingBoxOutline( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "gkg::Curve3dMap< float >& boundingBoxOutline ) const" );

}


void gkg::SceneModeler::getGlobalEvolvingMesh( 
                  const gkg::BoundingBox< float >& boundingBox,
                  gkg::MeshMap< int32_t, float, 3U >& globalEvolvingMesh ) const
{

  try
  {

    ////// computing the global numbers of vertices and polygons
    int32_t globalVertexCount = 0;
    int32_t globalPolygonCount = 0;
    bool evolving = false;
    int32_t rank = 0;
    std::map< std::string, gkg::RCPointer< gkg::EvolvingMesh > >::const_iterator
      m = _evolvingMeshes.begin(),
      me = _evolvingMeshes.end();
    while ( m != me )
    {

      int32_t croppedVertexCount = 0;
      int32_t croppedPolygonCount = 0;
      ( m->second )->evaluateCroppedMesh( boundingBox,
                                          croppedVertexCount,
                                          croppedPolygonCount );
      globalVertexCount += croppedVertexCount;
      globalPolygonCount += croppedPolygonCount;

      if ( m->second->storeVertexTrajectories() )
      {

        evolving = true;
        rank = ( m->second )->getVertexTrajectory( 0 ).getPointCount();

      }
      ++ m;

    }

    ////// preparing mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    int32_t r = 0;
    for ( r = 0; r < rank; r++ )
    {

      meshAccumulator.reserve( r,
                               globalVertexCount,
                               globalPolygonCount );

    }

    ////// collecting evolvingMesh meshes
    m = _evolvingMeshes.begin();
    while ( m != me )
    {

      // creating a new mesh
      gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > mesh;

      if ( evolving )
      {

        // case: evolving mesh
        mesh = ( m->second )->getEvolvingMesh();

      }
      else
      {

        // case: static mesh
        mesh = ( m->second )->getCroppedMesh( boundingBox );

      }

      // adding the evolving mesh to the mesh accumulator;
      meshAccumulator.add( *mesh );

      ++ m;

    }

    // filling the output mesh map
    globalEvolvingMesh.add( meshAccumulator );

  }
  GKG_CATCH( "void gkg::SceneModeler::getGlobalEvolvingMesh( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "gkg::MeshMap< int32_t, float, 3U >& globalEvolvingMesh ) const" );

}


void gkg::SceneModeler::simulate()
{

  try
  {

    int32_t step = 0;
    for ( step = 0; step < _stepCount; step++ )
    {

      if ( _verbose )
      {

        if ( ( ( step + 1 ) % 1000 == 0 ) ||
             ( step == 0 ) ||
             ( step == _stepCount - 1 ) )
        {

          if ( step )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << step + 1
                    << " / " << std::setw( 7 ) << _stepCount
                    << " ]" << std::flush;

        }

      }
      iterate( step );

    }

  }
  GKG_CATCH( "void gkg::SceneModeler::simulate()" );

}


void gkg::SceneModeler::iterate( int32_t step )
{

  try
  {

    // moving the evolvingMesh (can be parallelized)
    std::map< std::string, gkg::RCPointer< gkg::EvolvingMesh > >::const_iterator 
      m = _evolvingMeshes.begin(),
      me = _evolvingMeshes.end();
    while ( m != me )
    {
 
      m->second->move( step );
      ++ m;

    }

  }
  GKG_CATCH( "void gkg::SceneModeler::iterate( int32_t step )" );

}

