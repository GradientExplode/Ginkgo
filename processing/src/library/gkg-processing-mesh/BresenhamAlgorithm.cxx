#include <gkg-processing-mesh/BresenhamAlgorithm.h>
#include <gkg-processing-mesh/Polyhedron.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>
#include <set>


gkg::BresenhamAlgorithm::BresenhamAlgorithm(
                                   const gkg::BoundingBox< float >& boundingBox,
                                   const gkg::Vector3d< int32_t >& cacheSize )
                        : _boundingBox( boundingBox ),
                          _cacheSize( cacheSize ),
                          _cacheSizeMinusOne( cacheSize.x - 1,
                                              cacheSize.y - 1,
                                              cacheSize.z - 1 ),
                          _integerBoundingBox( 0, cacheSize.x - 1,
                                               0, cacheSize.y - 1,
                                               0, cacheSize.z - 1 )
{

  try
  {

    _lowerX = _boundingBox.getLowerX();
    _upperX = _boundingBox.getUpperX();
    _lowerY = _boundingBox.getLowerY();
    _upperY = _boundingBox.getUpperY();
    _lowerZ = _boundingBox.getLowerZ();
    _upperZ = _boundingBox.getUpperZ();

    _cacheVoxelFactorX = ( float )_cacheSize.x / ( _upperX - _lowerX );
    _cacheVoxelFactorY = ( float )_cacheSize.y / ( _upperY - _lowerY );
    _cacheVoxelFactorZ = ( float )_cacheSize.z / ( _upperZ - _lowerZ );

    _resolution.x = ( _upperX - _lowerX ) / _cacheSize.x;
    _resolution.y = ( _upperY - _lowerY ) / _cacheSize.y;
    _resolution.z = ( _upperZ - _lowerZ ) / _cacheSize.z;
    _minimumResolution = std::min( std::min( _resolution.x,
                                             _resolution.y ),
                                             _resolution.z );

  }
  GKG_CATCH( "gkg::BresenhamAlgorithm::BresenhamAlgorithm( "
             "gkg::RCPointer< gkg::SceneModeler > sceneModeler )" );

}


gkg::BresenhamAlgorithm::~BresenhamAlgorithm()
{
}


void gkg::BresenhamAlgorithm::getCacheVoxel(
                                         const gkg::Vector3d< float >& point,
                                         gkg::Vector3d< int32_t >& voxel ) const
{

  try
  {

    // x coordinate
    if ( point.x < _lowerX )
    {

      voxel.x = 0;

    }
    else if ( point.x >= _upperX )
    {

      voxel.x = _cacheSizeMinusOne.x;

    }
    else
    {

      voxel.x = ( int32_t )( ( point.x - _lowerX ) * _cacheVoxelFactorX );
      if ( voxel.x < 0 )
      {

        voxel.x = 0;

      }
      else if ( voxel.x > _cacheSizeMinusOne.x )
      {

        voxel.x = _cacheSizeMinusOne.x;

      }

    }

    // y coordinate
    if ( point.y < _lowerY )
    {

      voxel.y = 0;

    }
    else if ( point.y >= _upperY )
    {

      voxel.y = _cacheSizeMinusOne.y;

    }
    else
    {

      voxel.y = ( int32_t )( ( point.y - _lowerY ) * _cacheVoxelFactorY );
      if ( voxel.y < 0 )
      {

        voxel.y = 0;

      }
      else if ( voxel.y > _cacheSizeMinusOne.y )
      {

        voxel.y = _cacheSizeMinusOne.y;

      }

    }

    // z coordinate
    if ( point.z < _lowerZ )
    {

      voxel.z = 0;

    }
    else if ( point.z >= _upperZ )
    {

      voxel.z = _cacheSizeMinusOne.z;

    }
    else
    {

      voxel.z = ( int32_t )( ( point.z - _lowerZ ) * _cacheVoxelFactorZ );
      if ( voxel.z < 0 )
      {

        voxel.z = 0;

      }
      else if ( voxel.z > _cacheSizeMinusOne.z )
      {

        voxel.z = _cacheSizeMinusOne.z;

      }

    }

  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getCacheVoxel( "
             "const gkg::Vector3d< float >& point, "
             "gkg::Vector3d< int32_t >& voxel ) const" );

}


void gkg::BresenhamAlgorithm::getRealWorldPoint(
                                         const gkg::Vector3d< int32_t >& voxel,
                                         gkg::Vector3d< float >& point ) const
{

  try
  {

    point.x = _lowerX + ( float )voxel.x * _resolution.x;
    point.y = _lowerY + ( float )voxel.y * _resolution.y;
    point.z = _lowerZ + ( float )voxel.z * _resolution.z;

  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getRealWorldPoint( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::Vector3d< float >& point ) const" );

}


void gkg::BresenhamAlgorithm::getNeighbors(
                           const gkg::Vector3d< int32_t >& voxel,
                           gkg::Vector3d< int32_t > stride,
                           std::vector< gkg::Vector3d< int32_t > >& neighbors,
                           std::vector< bool >& neighborMasks ) const
{

  try
  {

    // resizing the neighborhood offsets and masks according to the stride
    gkg::Vector3d< int32_t > size( 2 * stride.x + 1,
                                   2 * stride.y + 1,
                                   2 * stride.z + 1 );
    neighbors.resize( size.x * size.y * size.z );
    neighborMasks.resize( size.x * size.y * size.z );

    // processing the neighborhood offsets and masks
    gkg::Vector3d< int32_t > offset;
    int32_t index = 0;
    int32_t x, y, z;
    for ( x = -stride.x; x <= stride.x; x++ )
    {

      offset.x = x;
      for ( y = -stride.y; y <= stride.y; y++ )
      {

        offset.y = y;
        for ( z = -stride.z; z <= stride.z; z++ )
        {

          offset.z = z;

          gkg::Vector3d< int32_t >& neighbor = neighbors[ index ];
          neighbor = voxel + offset;

          if ( ( neighbor.x >= 0 ) && ( neighbor.x <= _cacheSizeMinusOne.x ) &&
               ( neighbor.y >= 0 ) && ( neighbor.y <= _cacheSizeMinusOne.y ) &&
               ( neighbor.z >= 0 ) && ( neighbor.z <= _cacheSizeMinusOne.z ) )
          {

            neighborMasks[ index ] = true;

          }
          else
          {

            neighborMasks[ index ] = false;

          }

          ++ index;

        }

      }

    }  

  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getNeighbors( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::Vector3d< int32_t > stride, "
             "std::vector< gkg::Vector3d< int32_t > >& neighbors, "
             "std::vector< bool >& neighborMasks ) const" );

}


void gkg::BresenhamAlgorithm::getRayVoxels(
                     const gkg::Vector3d< float >& from,
                     const gkg::Vector3d< float >& to,
                     std::set< gkg::Vector3d< int32_t >,
                               gkg::Vector3dCompare< int32_t > >& voxels,
                     bool clearVoxelsAtBeginning ) const
{

  try
  {

    if ( clearVoxelsAtBeginning )
    {

      voxels.clear();

    }

    // processing initial and last voxel
    gkg::Vector3d< int32_t > fromVoxel, toVoxel;
    getCacheVoxel( from, fromVoxel );
    getCacheVoxel( to, toVoxel );

    voxels.insert( fromVoxel );

    if ( toVoxel != fromVoxel )
    {

      voxels.insert( toVoxel );

      // getting the length between the starting and ending points
      float length = ( to - from ).getNorm();

      // getting the segment count and the step
      int32_t segmentCount = ( int32_t )( length / _minimumResolution ) + 1;
      gkg::Vector3d< float > step = ( to - from ) / ( float )segmentCount;

      gkg::Vector3d< int32_t > currentVoxel = fromVoxel;
      gkg::Vector3d< float > newPoint = from;
      gkg::Vector3d< int32_t > newVoxel;
      gkg::Vector3d< int32_t > offset;
      gkg::Vector3d< int32_t > neighbor;
      int32_t s;

      for ( s = 1; s <= segmentCount; s++ )
      {

        // getting the current point and voxel
        newPoint += step;
        getCacheVoxel( newPoint, newVoxel );
        voxels.insert( newVoxel );

        // processing the offset and collecting the voxels
        offset = newVoxel - currentVoxel;

        if ( ( std::abs( offset.x ) +
               std::abs( offset.y ) +
               std::abs( offset.z ) ) > 1 )
        {

          neighbor.x = currentVoxel.x + offset.x;
          neighbor.y = currentVoxel.y;
          neighbor.z = currentVoxel.z;
          voxels.insert( neighbor ); 

          neighbor.x = currentVoxel.x;
          neighbor.y = currentVoxel.y + offset.y;
          neighbor.z = currentVoxel.z;
          voxels.insert( neighbor ); 

          neighbor.x = currentVoxel.x;
          neighbor.y = currentVoxel.y;
          neighbor.z = currentVoxel.z + offset.z;
          voxels.insert( neighbor ); 

          neighbor.x = currentVoxel.x + offset.x;
          neighbor.y = currentVoxel.y + offset.y;
          neighbor.z = currentVoxel.z;
          voxels.insert( neighbor ); 

          neighbor.x = currentVoxel.x + offset.x;
          neighbor.y = currentVoxel.y;
          neighbor.z = currentVoxel.z + offset.z;
          voxels.insert( neighbor ); 

          neighbor.x = currentVoxel.x;
          neighbor.y = currentVoxel.y + offset.y;
          neighbor.z = currentVoxel.z + offset.z;
          voxels.insert( neighbor ); 

        }
        // updating the current voxel
        currentVoxel = newVoxel;

      }

    }

  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getRayVoxels( "
             "const gkg::Vector3d< float >& from, "
             "const gkg::Vector3d< float >& to, "
             "std::set< gkg::Vector3d< int32_t >, "
             "gkg::Vector3dCompare< int32_t > >& voxels, "
             "bool clearVoxelsAtBeginning ) const" );

}


void gkg::BresenhamAlgorithm::getTriangleVoxels(
                                   const gkg::Vector3d< float >& vertex1,
                                   const gkg::Vector3d< float >& vertex2,
                                   const gkg::Vector3d< float >& vertex3,
                                   std::set< gkg::Vector3d< int32_t >,
                                             gkg::Vector3dCompare< int32_t > >&
                                                                  voxels,
                                   bool clearVoxelsAtBeginning ) const
{

  try
  {

    if ( clearVoxelsAtBeginning )
    {

      voxels.clear();

    }

    ////////////////////////////////////////////////////////////////////////////
    // First: collecting the voxels along the rays defined by the vertices
    ////////////////////////////////////////////////////////////////////////////

    // getting the voxels between vertex1 and vertex2
    getRayVoxels( vertex1, vertex2, voxels, false );

    // getting the voxels between vertex2 and vertex3
    getRayVoxels( vertex2, vertex3, voxels, false );

    // getting the voxels between vertex3 and vertex1
    getRayVoxels( vertex3, vertex1, voxels, false );

    ////////////////////////////////////////////////////////////////////////////
    // Second: collecting the voxels along the rays defined by a vertex
    // and points
    ////////////////////////////////////////////////////////////////////////////

    // getting the length of the three line segments defined by the triangle
    float segment12 = ( vertex1 - vertex2 ).getNorm();
    float segment23 = ( vertex2 - vertex3 ).getNorm();
    float segment31 = ( vertex3 - vertex1 ).getNorm();

    gkg::Vector3d< float > point;
    gkg::Vector3d< float > step;
    int32_t segmentCount = 0;
    int32_t s = 0;

    // getting the voxels between vertex3 and the points located on
    // the segment defined by vertex1 and vertex2
    point = vertex1;
    segmentCount = ( int32_t )( segment12 / _minimumResolution ) + 1;
    step = ( vertex2 - vertex1 ) / ( float )segmentCount;
    for ( s = 1; s < segmentCount; s++ )
    {

      point += step;
      getRayVoxels( vertex3, point, voxels, false );

    }

    // getting the voxels between vertex1 and the points located on
    // the segment defined by vertex2 and vertex3
    point = vertex2;
    segmentCount = ( int32_t )( segment23 / _minimumResolution ) + 1;
    step = ( vertex3 - vertex2 ) / ( float )segmentCount;

    for ( s = 1; s < segmentCount; s++ )
    {

      point += step;
      getRayVoxels( vertex1, point, voxels, false );

    }

    // getting the voxels between vertex2 and the points located on
    // the segment defined by vertex3 and vertex1
    point = vertex3;
    segmentCount = ( int32_t )( segment31 / _minimumResolution ) + 1;
    step = ( vertex1 - vertex3 ) / ( float )segmentCount;

    for ( s = 1; s < segmentCount; s++ )
    {

      point += step;
      getRayVoxels( vertex2, point, voxels, false );

    }

  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getTriangleVoxels( "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "const gkg::Vector3d< float >& vertex3, "
             "std::set< gkg::Vector3d< int32_t >, "
             "gkg::Vector3dCompare< int32_t > >& voxels, "
             "bool clearVoxelsAtBeginning ) const" );

}


void gkg::BresenhamAlgorithm::getThickTriangleVoxels(
                                   const gkg::Vector3d< float >& vertex1,
                                   const gkg::Vector3d< float >& vertex2,
                                   const gkg::Vector3d< float >& vertex3,
                                   float radiusOfInfluence,
                                   std::set< gkg::Vector3d< int32_t >,
                                             gkg::Vector3dCompare< int32_t > >&
                                                                  voxels,
                                   bool clearVoxelsAtBeginning ) const

{

  try
  {

    if ( clearVoxelsAtBeginning )
    {

      voxels.clear();

    }

//#if 0

    float usedRadiusOfInfluence = radiusOfInfluence;

    if ( usedRadiusOfInfluence == 0 )
    {

      usedRadiusOfInfluence = _minimumResolution / 10.0;

    }

    // getting the midpoint on each segment of the triangle
    gkg::Vector3d< float > midPoint1 = ( vertex2 + vertex3 ) / 2.0f;
    gkg::Vector3d< float > midPoint2 = ( vertex3 + vertex1 ) / 2.0f;
    gkg::Vector3d< float > midPoint3 = ( vertex1 + vertex2 ) / 2.0f;

    gkg::Vector3d< float >
      shift1 = ( ( vertex1 - midPoint1 ).normalize() ) * usedRadiusOfInfluence;
    gkg::Vector3d< float >
      shift2 = ( ( vertex2 - midPoint2 ).normalize() ) * usedRadiusOfInfluence;
    gkg::Vector3d< float >
      shift3 = ( ( vertex3 - midPoint3 ).normalize() ) * usedRadiusOfInfluence;

    // getting the normal vector of the plane defined by the vertices
    gkg::Vector3d< float >
      normal = ( ( vertex2 - vertex1 ).cross( vertex3 - vertex1 ) ).normalize();

    gkg::Polyhedron polyhedron;
    polyhedron.reserve( 6 );

    gkg::Vector3d< float > offset( _lowerX, _lowerY, _lowerZ );

    gkg::Vector3d< float > vertex1PlusShift1 = vertex1 - offset + shift1;
    gkg::Vector3d< float > vertex2PlusShift2 = vertex2 - offset + shift2;
    gkg::Vector3d< float > vertex3PlusShift3 = vertex3 - offset + shift3;
    gkg::Vector3d< float > 
      normalTimesRadiusOfInfluence = normal * usedRadiusOfInfluence;


    polyhedron.addVertex( vertex1PlusShift1 - normalTimesRadiusOfInfluence,
                          1, 3, 2 );
    polyhedron.addVertex( vertex2PlusShift2 - normalTimesRadiusOfInfluence,
                          4, 0, 2 );
    polyhedron.addVertex( vertex3PlusShift3 - normalTimesRadiusOfInfluence,
                          5, 1, 0 );
    polyhedron.addVertex( vertex1PlusShift1 + normalTimesRadiusOfInfluence,
                          0, 4, 5 );
    polyhedron.addVertex( vertex2PlusShift2 + normalTimesRadiusOfInfluence,
                          3, 1, 5 );
    polyhedron.addVertex( vertex3PlusShift3 + normalTimesRadiusOfInfluence,
                          4, 2, 3 );

    polyhedron.voxelize( _cacheSize, _resolution, voxels );

/*
    std::cout << std::endl;
    std::set< gkg::Vector3d< int32_t >,
              gkg::Vector3dCompare< int32_t > >::const_iterator
      ss = voxels.begin(),
      sse = voxels.end();
    while ( ss != sse )
    {

      std::cout << *ss << " " << std::flush;
      ++ ss;

    }
*/

//#endif

#if 0

    // FORMER CODE THAT WAS OPITMIZED USING POLYHEDRON

    if ( radiusOfInfluence == 0 )
    {

      getTriangleVoxels( vertex1,
                         vertex2,
                         vertex3,
                         voxels,
                         false );

    }
    else
    {

      // getting the midpoint on each segment of the triangle
      gkg::Vector3d< float > midPoint1 = ( vertex2 + vertex3 ) / 2.0f;
      gkg::Vector3d< float > midPoint2 = ( vertex3 + vertex1 ) / 2.0f;
      gkg::Vector3d< float > midPoint3 = ( vertex1 + vertex2 ) / 2.0f;

      gkg::Vector3d< float >
        shift1 = ( ( vertex1 - midPoint1 ).normalize() ) * radiusOfInfluence;
      gkg::Vector3d< float >
        shift2 = ( ( vertex2 - midPoint2 ).normalize() ) * radiusOfInfluence;
      gkg::Vector3d< float >
        shift3 = ( ( vertex3 - midPoint3 ).normalize() ) * radiusOfInfluence;

      // getting the normal vector of the plane defined by the vertices
      gkg::Vector3d< float >
        normal =
         ( ( vertex2 - vertex1 ).cross( vertex3 - vertex1 ) ).normalize();
      // 1) getting new vertices;
      // 2) moving the new triangle along its normal and collecting the voxels
      //    contained in the triangle for each movement;
      // 3) adding the voxels into a set to keep the voxel unique

      if ( ( 2 * radiusOfInfluence ) < _minimumResolution )
      {

        gkg::Vector3d< float > newVertex1, newVertex2, newVertex3;
        int32_t s;
        for ( s = 0; s < 3; s++ )
        {

          newVertex1 = vertex1 + shift1 + 
                       normal * ( radiusOfInfluence -
                                  radiusOfInfluence * ( float )s );
          newVertex2 = vertex2 + shift2 + 
                       normal * ( radiusOfInfluence -
                                  radiusOfInfluence * ( float )s );
          newVertex3 = vertex3 + shift3 + 
                       normal * ( radiusOfInfluence -
                                  radiusOfInfluence * ( float )s );

          getTriangleVoxels( newVertex1,
                             newVertex2,
                             newVertex3,
                             voxels,
                             false );

        }

      }
      else
      {

        int32_t segmentCount = ( int32_t )( radiusOfInfluence /
                                            _minimumResolution ) + 1;
        float step = radiusOfInfluence / ( float )segmentCount;

        // first adding the central triangle
        getTriangleVoxels( vertex1 + shift1,
                           vertex2 + shift2,
                           vertex3 + shift3,
                           voxels,
                           false );

        int32_t s;
        for ( s = 1; s <= segmentCount; s++ )
        {

          // along +normal direction
          getTriangleVoxels( vertex1 + shift1 + 
                             normal * step * ( float )s,
                             vertex2 + shift2 + 
                             normal * step * ( float )s,
                             vertex3 + shift3 + 
                             normal * step * ( float )s,
                             voxels,
                             false );

          // along -normal direction
          getTriangleVoxels( vertex1 + shift1 - 
                             normal * step * ( float )s,
                             vertex2 + shift2 - 
                             normal * step * ( float )s,
                             vertex3 + shift3 - 
                             normal * step * ( float )s,
                             voxels,
                             false );

        }

      }

    }

    std::cout << std::endl;
    std::set< gkg::Vector3d< int32_t >,
              gkg::Vector3dCompare< int32_t > >::const_iterator
      ss = voxels.begin(),
      sse = voxels.end();
    while ( ss != sse )
    {

      std::cout << *ss << " " << std::flush;
      ++ ss;

    }


#endif


  }
  GKG_CATCH( "void gkg::BresenhamAlgorithm::getThickTriangleVoxels( "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "const gkg::Vector3d< float >& vertex3, "
             "float radiusOfInfluence, "
             "std::set< gkg::Vector3d< int32_t >, "
             "gkg::Vector3dCompare< int32_t > >& voxels, "
             "bool clearVoxelsAtBeginning ) const" );

}

