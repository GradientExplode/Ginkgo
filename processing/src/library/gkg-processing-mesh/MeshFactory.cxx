#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-mesh/MeshTransformer_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-object/GenericObject.h>
#include <gkg-core-object/TypedObject.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::MeshFactory::MeshFactory()
{
}


gkg::MeshFactory::~MeshFactory()
{
}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getCylinder( const gkg::Vector3d< float >& center,
                               const gkg::Vector3d< float >& axis,
                               float length,
                               float radius1,
                               float radius2,
                               int32_t facetCount,
                               bool closed,
                               bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // getting point1 & point2
    gkg::Vector3d< float > cylinderAxis( axis );
    cylinderAxis.normalize();
    gkg::Vector3d< float > shift = cylinderAxis * length / 2.0f;
    gkg::Vector3d< float > point1 = center + shift;
    gkg::Vector3d< float > point2 = center - shift;

    // getting accessors to the vertex and polygon map(s)
    gkg::SiteMap< int32_t, float >& vertexMap = meshMap->vertices;
    gkg::PolygonMap< 3U >& polygonMap = meshMap->polygons;

    // allocation temporary vectors for vertices and polygons
    std::vector< gkg::Vector3d< float > > vertices;
    std::vector< gkg::Polygon< 3U > > polygons;

    if ( closed )
    {

      vertices.resize( facetCount * 2 + 2 );
      polygons.resize( facetCount * 4 );

    }
    else
    {

      vertices.resize( facetCount * 2 );
      polygons.resize( facetCount * 2 );

    }

    gkg::Vector3d< float > plane1, plane2;
    cylinderAxis.normalize();
    if ( cylinderAxis.z == 0.0 )
    {

      plane1.x = -cylinderAxis.y;
      plane1.y = cylinderAxis.x;
      plane1.z = 0.0;

    }
    else
    {

      plane1.x = 0.0;
      plane1.y = -cylinderAxis.z;
      plane1.z = cylinderAxis.y;

    }
    plane1.normalize();

    plane2 = cylinderAxis.cross( plane1 );
    plane2.normalize();

    gkg::Vector3d< float > normal;
    float angle = 0.0;
    int32_t f = 0;
    int32_t vertexCount = 0;
    int32_t polygonCount = 0;
    for ( f = 0; f < facetCount; f++ )
    {

      angle = M_PI * ( float )( ( f + 1 ) * 2 ) / ( float )facetCount;
      normal = plane1 * ( float )std::cos( angle ) +
               plane2 * ( float )std::sin( angle );
      vertices[ vertexCount ] = point1 + normal * radius1;
      ++ vertexCount;
      vertices[ vertexCount ] = point2 + normal * radius2;
      ++ vertexCount;

      polygons[ polygonCount ] =
        gkg::Polygon< 3U >( ( uint32_t )( f * 2 ) % ( facetCount * 2 ),
                            ( uint32_t )( f * 2 + 2 ) % ( facetCount * 2 ),
                            ( uint32_t )( f * 2 + 1 ) % ( facetCount * 2 )
                           );
      ++ polygonCount;

      polygons[ polygonCount ] =
        gkg::Polygon< 3U >( ( uint32_t )( f * 2 + 1 ) % ( facetCount * 2 ),
                            ( uint32_t )( f * 2 + 2 ) % ( facetCount * 2 ),
                            ( uint32_t )( f * 2 + 3 ) % ( facetCount * 2 )
                           );
      ++ polygonCount;

    }

    uint32_t actualVertexCount = ( uint32_t )vertexCount;
    if ( closed )
    {

      vertices[ vertexCount ] = point1;
      ++ vertexCount;

      vertices[ vertexCount ] = point2;
      ++ vertexCount;

      for ( f = 0; f < facetCount; f++ )
      {

        polygons[ polygonCount ] =
          gkg::Polygon< 3U >( ( uint32_t )( f * 2 + 2 ) % ( facetCount * 2 ),
                              actualVertexCount,
                              ( uint32_t )( f * 2 + 4 ) % ( facetCount * 2 )
                             );
        ++ polygonCount;

        polygons[ polygonCount ] =
          gkg::Polygon< 3U >( ( uint32_t )( f * 2 + 3 ) % ( facetCount * 2 ),
                              ( uint32_t )( f * 2 + 5 ) % ( facetCount * 2 ),
                              actualVertexCount + 1U
                             );
        ++ polygonCount;

      }

    }

    // storing vertices and polygons in the mesh map
    vertexMap.addSites( 0, vertices );
    polygonMap.addPolygons( 0, polygons );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getCylinder( "
             "const gkg::Vector3d< float >& center, "
             "const gkg::Vector3d< float >& axis, "
             "float length, "
             "float radius1, "
             "float radius2, "
             "int32_t facetCount, "
             "bool closed, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getGeneralizedCylinder(
                                      const LightCurve3d< float >& crossSection,
                                      float length,
                                      bool closed,
                                      bool randomRotation ) const
{

  try
  {

    // sanity check
    int32_t pointCount = crossSection.getPointCount();
    if ( pointCount < 2 )
    {

      throw std::runtime_error( "cross section must be a plane" );

    }

    // getting the normal of the cross section (i.e. cylinder axis)
    gkg::Vector3d< float > v0 = crossSection.getPoint( 0 );
    gkg::Vector3d< float > v1 = crossSection.getPoint( 1 );
    gkg::Vector3d< float > v2 = crossSection.getPoint( 2 );
    gkg::Vector3d< float > axis = ( v1 - v0 ).cross( v2 - v0 );
    if ( axis.getNorm2() == 0 )
    {

      throw std::runtime_error( "axis is a null vector" );

    }
    axis.normalize();

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // getting accessors to the vertex and polygon map(s)
    gkg::SiteMap< int32_t, float >& vertexMap = meshMap->vertices;
    gkg::PolygonMap< 3U >& polygonMap = meshMap->polygons;

    // allocation temporary vectors for vertices and polygons
    std::vector< gkg::Vector3d< float > > vertices;
    std::vector< gkg::Polygon< 3U > > polygons;

    if ( closed )
    {

      vertices.resize( pointCount * 2 + 2 );
      polygons.resize( pointCount * 4 );

    }
    else
    {

      vertices.resize( pointCount * 2 );
      polygons.resize( pointCount * 2 );

    }

    // adding the vertices and polygons
    gkg::Vector3d< float > point;
    gkg::Vector3d< float > shift = axis * length / 2.0f;
    int32_t p = 0;
    int32_t vertexCount = 0;
    int32_t polygonCount = 0;
    for ( p = 0; p < pointCount; p++ )
    {

      point = crossSection.getPoint( p );

      vertices[ vertexCount ] = point + shift;
      ++ vertexCount;
      vertices[ vertexCount ] = point - shift;
      ++ vertexCount;

      polygons[ polygonCount ] =
        gkg::Polygon< 3U >( ( uint32_t )( p * 2 ) % ( pointCount * 2 ),
                            ( uint32_t )( p * 2 + 2 ) % ( pointCount * 2 ),
                            ( uint32_t )( p * 2 + 1 ) % ( pointCount * 2 )
                           );
      ++ polygonCount;

      polygons[ polygonCount ] =
        gkg::Polygon< 3U >( ( uint32_t )( p * 2 + 1 ) % ( pointCount * 2 ),
                            ( uint32_t )( p * 2 + 2 ) % ( pointCount * 2 ),
                            ( uint32_t )( p * 2 + 3 ) % ( pointCount * 2 )
                           );
      ++ polygonCount;

    }

    uint32_t actualVertexCount = ( uint32_t )vertexCount;
    if ( closed )
    {

      gkg::Vector3d< float > centerOfMass = crossSection.getCenterOfMass();

      vertices[ vertexCount ] = centerOfMass + shift;
      ++ vertexCount;

      vertices[ vertexCount ] = centerOfMass - shift;
      ++ vertexCount;

      for ( p = 0; p < pointCount; p++ )
      {

        polygons[ polygonCount ] =
          gkg::Polygon< 3U >( ( uint32_t )( p * 2 + 2 ) % ( pointCount * 2 ),
                              actualVertexCount,
                              ( uint32_t )( p * 2 + 4 ) % ( pointCount * 2 )
                             );
        ++ polygonCount;

        polygons[ polygonCount ] =
          gkg::Polygon< 3U >( ( uint32_t )( p * 2 + 3 ) % ( pointCount * 2 ),
                              ( uint32_t )( p * 2 + 5 ) % ( pointCount * 2 ),
                              actualVertexCount + 1U
                             );
        ++ polygonCount;

      }

    }

    // storing vertices and polygons in the mesh map
    vertexMap.addSites( 0, vertices );
    polygonMap.addPolygons( 0, polygons );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getGeneralizedCylinder( "
             "const LightCurve3d< float >& crossSection, "
             "float length, "
             "bool closed, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getFiber( const gkg::LightCurve3d< float >& lightCurve3d,
                            const std::vector< float >& radii,
                            int32_t vertexCountPerPlane,
                            bool randomRotation,
                            int32_t destinationRank ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // allocating vectors of vertex and polygon maps
    int32_t pointCount = lightCurve3d.getPointCount();
    std::vector< std::vector< gkg::Vector3d< float > > > vertices;
    vertices.resize( pointCount );

    std::vector< std::vector< gkg::Polygon< 3U > > > polygons;
    int32_t segmentCount = pointCount - 1;
    polygons.resize( segmentCount );

    // getting the vertices and polygons mesh for each segment
    int32_t facetCountPerSegment = 2 * vertexCountPerPlane;
    gkg::Vector3d< float > cylinderAxis, plane1, plane2, normal;
    gkg::Vector3d< float > currentPoint, nextPoint;
    float angle = 0.0;
    int32_t polygonIndex = 0;
    int32_t s = 0;
    int32_t v = 0;
    for ( s = 0; s < segmentCount; s++ )
    {

      // allocating vertices[ plane ][ vertex ]
      vertices[ s ].resize( vertexCountPerPlane );
      if ( s == ( segmentCount - 1 ) )
      {

        // the last plane
        vertices[ s + 1 ].resize( vertexCountPerPlane );

      }
      // allocating polygons[ segment ][ polygon ]
      polygons[ s ].resize( facetCountPerSegment );

      // processing axis and planes
      nextPoint = lightCurve3d.getPoint( s + 1 );
      currentPoint = lightCurve3d.getPoint( s );
      cylinderAxis = nextPoint - currentPoint;
      cylinderAxis.normalize();
      if ( cylinderAxis.z == 0 )
      {

        plane1.x = -cylinderAxis.y;
        plane1.y = cylinderAxis.x;
        plane1.z = 0.0;

      }
      else
      {

        plane1.x = 0.0;
        plane1.y = -cylinderAxis.z;
        plane1.z = cylinderAxis.y;

      } 
      plane1.normalize();

      plane2 = cylinderAxis.cross( plane1 );
      plane2.normalize();

      for ( v = 0; v < vertexCountPerPlane; v++ )
      {

        angle = M_PI * ( float )( ( v + 1 ) * 2 ) /
                       ( float )vertexCountPerPlane;
        normal = plane1 * ( float )std::cos( angle ) +
                 plane2 * ( float )std::sin( angle );

        // filling vertices
        vertices[ s ][ v ] = currentPoint + normal * radii[ s ];
        if ( s == ( segmentCount - 1 ) )
        {

          // the last plane
          vertices[ s + 1 ][ v ] = nextPoint + normal * radii[ s + 1 ];

        }

        // filling polygons
        polygons[ s ][ polygonIndex ] =
          gkg::Polygon< 3U >( ( uint32_t )( v * 2 ) % facetCountPerSegment,
                              ( uint32_t )( v * 2 + 2 ) % facetCountPerSegment,
                              ( uint32_t )( v * 2 + 1 ) % facetCountPerSegment
                             );
        ++ polygonIndex;

        polygons[ s ][ polygonIndex ] =
          gkg::Polygon< 3U >( ( uint32_t )( v * 2 + 1 ) % facetCountPerSegment,
                              ( uint32_t )( v * 2 + 2 ) % facetCountPerSegment,
                              ( uint32_t )( v * 2 + 3 ) % facetCountPerSegment
                             );
        ++ polygonIndex;

      }
      // reset polygonIndex
      polygonIndex = 0;

    }

    // reordering vertices for each segment
    std::vector< std::vector< gkg::Vector3d< float > > > reorderedVertices;
    reorderedVertices.resize( segmentCount );

    int32_t vertexCountPerSegment = 2 * vertexCountPerPlane;
    int32_t vertexIndex = 0;
    for ( s = 0; s < segmentCount; s++ )
    {

      // allocating reorderedVertices[ segment ][ polygon ]
      reorderedVertices[ s ].resize( vertexCountPerSegment );

      for ( v = 0; v < vertexCountPerPlane; v++ )
      {

        reorderedVertices[ s ][ vertexIndex ] = vertices[ s ][ v ];
        ++ vertexIndex;

        reorderedVertices[ s ][ vertexIndex ] = vertices[ s + 1 ][ v ];
        ++ vertexIndex;

      }
      // reset vertex index
      vertexIndex = 0;

    }

    // preparing mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             vertexCountPerPlane * pointCount,
                             facetCountPerSegment * segmentCount );

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > cylinderMesh;
    for ( s = 0; s < segmentCount; s++ )
    {

      cylinderMesh.reset( new gkg::MeshMap< int32_t, float, 3U >() );
      cylinderMesh->vertices.addSites( 0, reorderedVertices[ s ] );
      cylinderMesh->polygons.addPolygons( 0, polygons[ s ] );

      meshAccumulator.add( *cylinderMesh );

    }
    meshMap->add( meshAccumulator );

    ////// processing the mesh map to create the output fiber mesh map
    // allocating the output fiber meshmap
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      fiberMeshMap( new gkg::MeshMap< int32_t, float, 3U > );

    // collecting the vertices and polygons from the meshmap
    std::list< gkg::Vector3d< float > >
      vertexList = meshMap->vertices.getSites( 0 );
    std::list< gkg::Polygon< 3U > >
      polygonList = meshMap->polygons.getPolygons( 0 );

    // collecting a set of unique vertices
    std::set< gkg::Vector3d< float >,
              gkg::Vector3dCompare< float > > keptVertices;

    std::list< gkg::Vector3d< float > >::const_iterator
      i = vertexList.begin(),
      ie = vertexList.end();
    while ( i != ie )
    {

      keptVertices.insert( *i );
      ++ i;

    }

    // creating the old index to new vertex index lookup table
    std::map< uint32_t, uint32_t > oldIndexToNewIndexLut;
    uint32_t oldIndex = 0;
    uint32_t newIndex = 0;

    std::set< gkg::Vector3d< float >,
              gkg::Vector3dCompare< float > >::const_iterator
      k = keptVertices.begin(),
      ke = keptVertices.end();
    i = vertexList.begin();
    while ( i != ie )
    {

      k = keptVertices.begin();
      while ( k != ke )
      {

        if ( ( ( *i ).x == ( *k ).x ) &&
             ( ( *i ).y == ( *k ).y ) &&
             ( ( *i ).z == ( *k ).z ) )
        {

          oldIndexToNewIndexLut[ oldIndex ] = newIndex;
          break;

        }
        ++ newIndex;
        ++ k;

      }
      // reset the new index
      newIndex = 0;

      ++ oldIndex;
      ++ i;

    }

    // updating the polygon indices
    uint32_t vi = 0;
    std::list< gkg::Polygon< 3U > >::iterator
      pl = polygonList.begin(),
      ple = polygonList.end();
    while ( pl != ple )
    {

      for ( vi = 0U; vi < 3U; vi++ )
      {

        pl->indices[ vi ] = oldIndexToNewIndexLut[ pl->indices[ vi ] ];

      }
      ++ pl;

    }

    // filling the vertices and polygons
    fiberMeshMap->vertices.addSites( destinationRank, keptVertices );
    fiberMeshMap->polygons.addPolygons(
              fiberMeshMap->vertices.getIndex( destinationRank ), polygonList );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *fiberMeshMap, rotation, *fiberMeshMap );

    }

    return fiberMeshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getfiber( "
             "const gkg::LightCurve3d< float > >& lightCurve3d, "
             "const std::vector< float >& radii, "
             "int32_t vertexCountPerPlane, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getEllipsoid( const gkg::Vector3d< float >& center,
                                const gkg::Vector3d< float >& e1,
                                const gkg::Vector3d< float >& e2,
                                const gkg::Vector3d< float >& e3,
                                float radius1,
                                float radius2,
                                float radius3,
                                int32_t sphereVertexCount,
                                bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building the mesh of a sphere using the convex hull
    gkg::ElectrostaticOrientationSet orientationSet( sphereVertexCount, false );


    gkg::ConvexHull::getInstance().addConvexHull( 
                                               orientationSet.getOrientations(),
                                               0,
                                               *meshMap );

    // preparing scaling
    gkg::Scaling3d< float > scaling( radius1, radius2, radius3 );

    // preparing mesh scaler and transformer
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
    meshTransformer.transform( *meshMap, scaling, *meshMap );

    // processing translation
    gkg::Translation3d< float > translation;
    translation.setDirectTranslation( center.x,
                                      center.y,
                                      center.z );
    meshTransformer.transform( *meshMap, translation, *meshMap );

    // computing rotation
    gkg::Rotation3d< float > 
      rotation( gkg::getXYZAnglesFromTrieder( e1, e2, e3 ) );
    meshTransformer.transform( *meshMap, rotation, *meshMap );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getEllipsoid( "
             "const gkg::Vector3d< float >& center, "
             "const gkg::Vector3d< float >& e1, "
             "const gkg::Vector3d< float >& e2, "
             "const gkg::Vector3d< float >& e3, "
             "float radius1, "
             "float radius2, "
             "float radius3, "
             "int32_t sphereVertexCount, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getSphere( const gkg::Vector3d< float >& center,
                             float radius,
                             int32_t sphereVertexCount ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building the mesh of a sphere using the convex hull
    gkg::ElectrostaticOrientationSet orientationSet( sphereVertexCount, false );

    gkg::ConvexHull::getInstance().addConvexHull( 
                                               orientationSet.getOrientations(),
                                               0,
                                               *meshMap );

    // preparing scaling
    gkg::Scaling3d< float > scaling( radius, radius, radius );

    // preparing mesh scaler and transformer
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
    meshTransformer.transform( *meshMap, scaling, *meshMap );

    // processing translation
    gkg::Translation3d< float > translation;
    translation.setDirectTranslation( center.x,
                                      center.y,
                                      center.z );
    meshTransformer.transform( *meshMap, translation, *meshMap );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getSphere( "
             "const gkg::Vector3d< float >& center, "
             "float radius, "
             "int32_t sphereVertexCount ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getStar( const gkg::Vector3d< float >& center,
                           float radius,
                           int32_t peakCount,
                           const std::vector< float >& peakMagnitudes,
                           const std::vector< float >& peakApertureAngles,
                           int32_t sphereVertexCount,
                           bool sharpPeak,
                           bool symmetricPeak,
                           bool randomRotation ) const
{

  try
  {

    // sanity checks
    if ( ( int32_t )peakMagnitudes.size() != peakCount )
    {

      throw std::runtime_error( "inconsistent peak magnitudes and count" );

    }

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building the mesh of a sphere using the convex hull
    gkg::ElectrostaticOrientationSet orientationSet( sphereVertexCount, false );

    gkg::ConvexHull::getInstance().addConvexHull( 
                                               orientationSet.getOrientations(),
                                               0,
                                               *meshMap );

    // building the peaks
    gkg::RCPointer< gkg::OrientationSet > peakSet;
    if ( symmetricPeak )
    {

      peakSet.reset( new gkg::ElectrostaticOrientationSet( peakCount, false ) );

    }
    else
    {

      peakSet.reset( new gkg::ElectrostaticOrientationSet( peakCount, true ) );

    }

    // processing scaling
    std::vector< float > scaling( sphereVertexCount );

    int32_t o = 0;
    int32_t p = 0;
    float scale = 0.0;
    float angle = 0.0;
    for ( o = 0; o < sphereVertexCount; o++ )
    {

      const gkg::Vector3d< float >& vertex = orientationSet.getOrientation( o );
      scale = radius;
      for ( p = 0; p < peakCount; p++ )
      {

        angle = gkg::getVectorAngles( peakSet->getOrientation( p ), vertex );
        if ( sharpPeak )
        {

          scale += peakMagnitudes[ p ] * std::exp( - ( angle * angle ) /
                                                  ( peakApertureAngles[ p ] *
                                                    peakApertureAngles[ p ] ) );

        }
        else
        {

          scale += peakMagnitudes[ p ] * std::exp( - angle /
                                                     peakApertureAngles[ p ] );

        }

      }

      scaling[ o ] = scale;

    }

    // preparing mesh scaler and transformer
    gkg::MeshScaler< int32_t, float, 3U > meshScaler;
    meshScaler.scale( *meshMap, 0, scaling, *meshMap );

    // preparing mesh scaler and transformer
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    // processing translation
    gkg::Translation3d< float > translation;
    translation.setDirectTranslation( center.x,
                                      center.y,
                                      center.z );
    meshTransformer.transform( *meshMap, translation, *meshMap );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getStar( "
             "const gkg::Vector3d< float >& center, "
             "float radius, "
             "int32_t peakCount, "
             "const std::vector< float >& peakMagnitudes, "
             "const std::vector< float >& peakApertureAngles, "
             "int32_t sphereVertexCount, "
             "bool sharpPeak, "
             "bool symmetricPeak, "
             "bool randomRotation) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getBox( const gkg::Vector3d< float >& lowerPoint,
                          const gkg::Vector3d< float >& upperPoint,
                          bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    gkg::SiteMap< int32_t, float >& vertexMap = meshMap->vertices;
    gkg::PolygonMap< 3U >& polygonMap = meshMap->polygons;

    // adding vertices
    vertexMap.addSite( 0, gkg::Vector3d< float >( lowerPoint.x,
                                                  lowerPoint.y,
                                                  lowerPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( upperPoint.x,
                                                  lowerPoint.y,
                                                  lowerPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( lowerPoint.x,
                                                  upperPoint.y,
                                                  lowerPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( upperPoint.x,
                                                  upperPoint.y,
                                                  lowerPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( lowerPoint.x,
                                                  lowerPoint.y,
                                                  upperPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( upperPoint.x,
                                                  lowerPoint.y,
                                                  upperPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( lowerPoint.x,
                                                  upperPoint.y,
                                                  upperPoint.z ) );
    vertexMap.addSite( 0, gkg::Vector3d< float >( upperPoint.x,
                                                  upperPoint.y,
                                                  upperPoint.z ) );

    // adding polygons
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 1, 7, 5 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 1, 3, 7 ) );

    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 3, 6, 7 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 3, 2, 6 ) );

    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 0, 4, 6 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 0, 6, 2 ) );

    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 0, 5, 4 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 0, 1, 5 ) );

    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 5, 7, 6 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 5, 6, 4 ) );

    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 2, 3, 1 ) );
    polygonMap.addPolygon( vertexMap.getIndex( 0 ),
                           gkg::Polygon< 3U >( 0, 2, 1 ) );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getBox( "
             "const gkg::Vector3d< float >& lowerPoint, "
             "const gkg::Vector3d< float >& upperPoint, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfCylinders(
                                     const gkg::Vector3d< float >& center,
                                     const gkg::Vector3d< float >& axis,
                                     float length,
                                     float radius1,
                                     float radius2,
                                     int32_t facetCount,
                                     bool closed,
                                     const gkg::Vector3d< float >& networkAxis1,
                                     const gkg::Vector3d< float >& networkAxis2,
                                     float networkSpacing1,
                                     float networkSpacing2,
                                     int32_t networkCount1,
                                     int32_t networkCount2,
                                     bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a cylindrical mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getCylinder( center,
                          axis,
                          length,
                          radius1,
                          radius2,
                          facetCount,
                          closed,
                          randomRotation );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;

    gkg::Vector3d< float > offset, o1, o2;
    int32_t n1, n2;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        offset = o1 + o2;

        // processing translation
        translation.setDirectTranslation( offset.x,
                                          offset.y,
                                          offset.z );

        // processing local mesh map
        gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );
        meshTransformer.transform( localMeshMap, translation,
                                   localMeshMap );

        // collecting local mesh map
        meshAccumulator.add( localMeshMap );

      }

    }
    meshMap->add( meshAccumulator );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfCylinders( "
             "const gkg::Vector3d< float >& center, "
             "const gkg::Vector3d< float >& axis, "
             "float length, "
             "float radius1, "
             "float radius2, "
             "int32_t facetCount, "
             "bool closed, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getNetworkOfGeneralizedCylinders(
                                 const gkg::LightCurve3d< float >& crossSection,
                                 float length,
                                 bool closed,
                                 const gkg::Vector3d< float >& networkAxis1,
                                 const gkg::Vector3d< float >& networkAxis2,
                                 float networkSpacing1,
                                 float networkSpacing2,
                                 int32_t networkCount1,
                                 int32_t networkCount2,
                                 bool randomRotation ) const
{

  try
  {

    // allocating an output mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U > );

    // building a generalized cylinder mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getGeneralizedCylinder( crossSection,
                                     length,
                                     closed,
                                     randomRotation );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting the total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;

    gkg::Vector3d< float > offset, o1, o2;
    int32_t n1, n2;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        offset = o1 + o2;

        // processing translation
        translation.setDirectTranslation( offset.x,
                                          offset.y,
                                          offset.z );

        // processing local mesh map
        gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );
        meshTransformer.transform( localMeshMap, translation,
                                   localMeshMap );

        // collecting local mesh map
        meshAccumulator.add( localMeshMap );

      }

    }
    meshMap->add( meshAccumulator );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;    

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfGeneralizedCylinders( "
             "const gkg::LightCurve3d< float >& crossSection, "
             "float length, "
             "bool closes, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getNetworkOfFibers(
                                 const gkg::LightCurve3d< float >& lightCurve3d,
                                 const std::vector< float >& radii,
                                 int32_t vertexCountPerPlane,
                                 const gkg::Vector3d< float >& networkAxis1,
                                 const gkg::Vector3d< float >& networkAxis2,
                                 float networkSpacing1,
                                 float networkSpacing2,
                                 int32_t networkCount1,
                                 int32_t networkCount2,
                                 bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a cylindrical mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getFiber( lightCurve3d,
                       radii,
                       vertexCountPerPlane,
                       randomRotation );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;

    gkg::Vector3d< float > offset, o1, o2;
    int32_t n1, n2;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        offset = o1 + o2;

        // processing translation
        translation.setDirectTranslation( offset.x,
                                          offset.y,
                                          offset.z );

        // processing local mesh map
        gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );
        meshTransformer.transform( localMeshMap, translation,
                                   localMeshMap );

        // collecting local mesh map
        meshAccumulator.add( localMeshMap );

      }

    }
    meshMap->add( meshAccumulator );

    // preparing spatial rotation if required
    if ( randomRotation )
    {

      // generating a random rotation angle
      std::vector< gkg::Vector3d< float > > angle = getRotationAngles( 1 );
      gkg::Rotation3d< float > rotation( angle[ 0 ] );

      // processing rotation
      gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;
      meshTransformer.transform( *meshMap, rotation, *meshMap );

    }

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfFibers( "
             "const gkg::LightCurve3d< float >& lightCurve3d, "
             "const std::vector< float >& radii, "
             "int32_t vertexCountPerPlane, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfEllipsoids(
                                     const gkg::Vector3d< float >& center,
                                     const gkg::Vector3d< float >& e1,
                                     const gkg::Vector3d< float >& e2,
                                     const gkg::Vector3d< float >& e3,
                                     float radius1,
                                     float radius2,
                                     float radius3,
                                     int32_t sphereVertexCount,
                                     const gkg::Vector3d< float >& networkAxis1,
                                     const gkg::Vector3d< float >& networkAxis2,
                                     const gkg::Vector3d< float >& networkAxis3,
                                     float networkSpacing1,
                                     float networkSpacing2,
                                     float networkSpacing3,
                                     int32_t networkCount1,
                                     int32_t networkCount2,
                                     int32_t networkCount3,
                                     bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a ellipsoid mesh at the origin
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getEllipsoid( gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                           e1,
                           e2,
                           e3,
                           radius1,
                           radius2,
                           radius3,
                           sphereVertexCount,
                           randomRotation );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2 * networkCount3;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // preparing rotation angles if required
    std::vector< gkg::Vector3d< float > > rotationAngles;
    if ( randomRotation )
    {

      rotationAngles = getRotationAngles( globalMeshCount );

    }

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    gkg::Vector3d< float > v3 = networkAxis3;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;
    v3 = v3.normalize() * networkSpacing3;

    gkg::Vector3d< float > offset, o1, o2, o3;
    int32_t angleIndex = 0;
    int32_t n1, n2, n3;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        for ( n3 = 0; n3 < networkCount3; n3++ )
        {

          o3 = v3 * ( float )n3;
          offset = center + o1 + o2 + o3;

          // allocating the local mesh map
          gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );

          // processing random rotation if required
          if ( randomRotation )
          {

            // processing rotation
            gkg::Rotation3d< float > rotation( rotationAngles[ angleIndex ] );
            meshTransformer.transform( localMeshMap, rotation,
                                       localMeshMap );

            // updating the index for the rotation angle
            ++angleIndex;

          }

          // processing translation
          translation.setDirectTranslation( offset.x,
                                            offset.y,
                                            offset.z );

          // processing local mesh map
          meshTransformer.transform( localMeshMap, translation,
                                     localMeshMap );

          // collecting local mesh map
          meshAccumulator.add( localMeshMap );

        }

      }

    }
    meshMap->add( meshAccumulator );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >  "
             "gkg::MeshFactory::getNetworkOfEllipsoids( "
             "const gkg::Vector3d< float >& center, "
             "const gkg::Vector3d< float >& e1, "
             "const gkg::Vector3d< float >& e2, "
             "const gkg::Vector3d< float >& e3, "
             "float radius1, "
             "float radius2, "
             "float radius3, "
             "int32_t sphereVertexCount, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "const gkg::Vector3d< float >& networkAxis3, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "float networkSpacing3, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "int32_t networkCount3, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfSpheres( 
                                     const gkg::Vector3d< float >& center,
                                     float radius,
                                     int32_t sphereVertexCount,
                                     const gkg::Vector3d< float >& networkAxis1,
                                     const gkg::Vector3d< float >& networkAxis2,
                                     const gkg::Vector3d< float >& networkAxis3,
                                     float networkSpacing1,
                                     float networkSpacing2,
                                     float networkSpacing3,
                                     int32_t networkCount1,
                                     int32_t networkCount2,
                                     int32_t networkCount3 ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a sphere mesh
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getSphere( center,
                        radius,
                        sphereVertexCount );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2 * networkCount3;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    gkg::Vector3d< float > v3 = networkAxis3;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;
    v3 = v3.normalize() * networkSpacing3;

    gkg::Vector3d< float > offset, o1, o2, o3;
    int32_t n1, n2, n3;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        for ( n3 = 0; n3 < networkCount3; n3++ )
        {

          o3 = v3 * ( float )n3;
          offset = o1 + o2 + o3;

          // processing translation
          translation.setDirectTranslation( offset.x,
                                            offset.y,
                                            offset.z );

          // processing local mesh map
          gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );
          meshTransformer.transform( localMeshMap, translation,
                                     localMeshMap );

          // collecting local mesh map
          meshAccumulator.add( localMeshMap );

        }

      }

    }
    meshMap->add( meshAccumulator );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfSpheres(  "
             "const gkg::Vector3d< float >& center, "
             "float radius, "
             "int32_t sphereVertexCount, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "const gkg::Vector3d< float >& networkAxis3, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "float networkSpacing3, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "int32_t networkCount3 ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float ,3U > >
gkg::MeshFactory::getNetworkOfStars(
                                 const gkg::Vector3d< float >& center,
                                 float radius,
                                 int32_t peakCount,
                                 const std::vector< float >& peakMagnitudes,
                                 const std::vector< float >& peakApertureAngles,
                                 int32_t sphereVertexCount,
                                 bool sharpPeak,
                                 bool symmetricPeak,
                                 const gkg::Vector3d< float >& networkAxis1,
                                 const gkg::Vector3d< float >& networkAxis2,
                                 const gkg::Vector3d< float >& networkAxis3,
                                 float networkSpacing1,
                                 float networkSpacing2,
                                 float networkSpacing3,
                                 int32_t networkCount1,
                                 int32_t networkCount2,
                                 int32_t networkCount3,
                                 bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a star mesh at the origin
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getStar( gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                      radius,
                      peakCount,
                      peakMagnitudes,
                      peakApertureAngles,
                      sphereVertexCount,
                      sharpPeak,
                      symmetricPeak,
                      randomRotation );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2 * networkCount3;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // preparing rotation angles if required
    std::vector< gkg::Vector3d< float > > rotationAngles;
    if ( randomRotation )
    {

      rotationAngles = getRotationAngles( globalMeshCount );

    }

    // looping over the network
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    gkg::Vector3d< float > v3 = networkAxis3;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;
    v3 = v3.normalize() * networkSpacing3;

    gkg::Vector3d< float > offset, o1, o2, o3;
    int32_t angleIndex = 0;
    int32_t n1, n2, n3;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        for ( n3 = 0; n3 < networkCount3; n3++ )
        {

          o3 = v3 * ( float )n3;
          offset = center + o1 + o2 + o3;

          // allocating the local mesh map
          gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );

          // processing random rotation if required
          if ( randomRotation )
          {

            // processing rotation
            gkg::Rotation3d< float > rotation( rotationAngles[ angleIndex ] );
            meshTransformer.transform( localMeshMap, rotation,
                                       localMeshMap );

            // updating the index for the rotation angle
            ++angleIndex;

          }

          // processing translation
          translation.setDirectTranslation( offset.x,
                                            offset.y,
                                            offset.z );

          // processing local mesh map
          meshTransformer.transform( localMeshMap, translation,
                                     localMeshMap );

          // collecting local mesh map
          meshAccumulator.add( localMeshMap );

        }

      }

    }
    meshMap->add( meshAccumulator );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float ,3U > > "
             "gkg::MeshFactory::getNetworkOfStars( "
             "const gkg::Vector3d< float >& center, "
             "float radius, "
             "int32_t peakCount, "
             "const std::vector< float >& peakMagnitudes, "
             "const std::vector< float >& peakApertureAngles, "
             "int32_t sphereVertexCount, "
             "bool sharpPeak, "
             "bool symmetricPeak, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "const gkg::Vector3d< float >& networkAxis3, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "float networkSpacing3, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "int32_t networkCount3, "
             "bool randomRotation ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getNetworkOfBoxes( const gkg::Vector3d< float >& lowerPoint,
                                     const gkg::Vector3d< float >& upperPoint,
                                     const gkg::Vector3d< float >& networkAxis1,
                                     const gkg::Vector3d< float >& networkAxis2,
                                     const gkg::Vector3d< float >& networkAxis3,
                                     float networkSpacing1,
                                     float networkSpacing2,
                                     float networkSpacing3,
                                     int32_t networkCount1,
                                     int32_t networkCount2,
                                     int32_t networkCount3,
                                     bool randomRotation ) const
{

  try
  {

    // allocating an output mesh map
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      meshMap( new gkg::MeshMap< int32_t, float, 3U >() );

    // building a star mesh at the origin
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      mesh = getBox( lowerPoint,
                     upperPoint,
                     0 );

    int32_t meshVertexCount = mesh->vertices.getSiteCount( 0 );
    int32_t meshPolygonCount = mesh->polygons.getPolygonCount( 0 );

    // getting total mesh count
    int32_t globalMeshCount = networkCount1 * networkCount2 * networkCount3;

    // allocating mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             meshVertexCount * globalMeshCount,
                             meshPolygonCount * globalMeshCount );

    // preparing spatial transformation
    gkg::Translation3d< float > translation;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    // preparing rotation angles if required
    std::vector< gkg::Vector3d< float > > rotationAngles;
    if ( randomRotation )
    {

      rotationAngles = getRotationAngles( globalMeshCount );

    }

    // looping over the network
    gkg::Vector3d< float > boxCenter( 0.5 * ( upperPoint.x - lowerPoint.x ),
                                      0.5 * ( upperPoint.y - lowerPoint.y ),
                                      0.5 * ( upperPoint.z - lowerPoint.z ) );
    gkg::Vector3d< float > v1 = networkAxis1;
    gkg::Vector3d< float > v2 = networkAxis2;
    gkg::Vector3d< float > v3 = networkAxis3;
    v1 = v1.normalize() * networkSpacing1;
    v2 = v2.normalize() * networkSpacing2;
    v3 = v3.normalize() * networkSpacing3;

    gkg::Vector3d< float > offset, o1, o2, o3;
    int32_t angleIndex = 0;
    int32_t n1, n2, n3;
    for ( n1 = 0; n1 < networkCount1; n1++ )
    {

      o1 = v1 * ( float )n1;
      for ( n2 = 0; n2 < networkCount2; n2++ )
      {

        o2 = v2 * ( float )n2;
        for ( n3 = 0; n3 < networkCount3; n3++ )
        {

          o3 = v3 * ( float )n3;
          offset = boxCenter + o1 + o2 + o3;

          // allocating the local mesh map
          gkg::MeshMap< int32_t, float, 3U > localMeshMap( *mesh );

          // processing random rotation if required
          if ( randomRotation )
          {

            // processing rotation
            gkg::Rotation3d< float > rotation( rotationAngles[ angleIndex ] );
            meshTransformer.transform( localMeshMap, rotation,
                                       localMeshMap );

            // updating the index for the rotation angle
            ++angleIndex;

          }

          // processing translation
          translation.setDirectTranslation( offset.x,
                                            offset.y,
                                            offset.z );

          // processing local mesh map
          meshTransformer.transform( localMeshMap, translation,
                                     localMeshMap );

          // collecting local mesh map
          meshAccumulator.add( localMeshMap );

        }

      }

    }
    meshMap->add( meshAccumulator );

    return meshMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfBoxes( "
             "const gkg::Vector3d< float >& lowerPoint, "
             "const gkg::Vector3d< float >& upperPoint, "
             "const gkg::Vector3d< float >& networkAxis1, "
             "const gkg::Vector3d< float >& networkAxis2, "
             "const gkg::Vector3d< float >& networkAxis3, "
             "float networkSpacing1, "
             "float networkSpacing2, "
             "float networkSpacing3, "
             "int32_t networkCount1, "
             "int32_t networkCount2, "
             "int32_t networkCount3, "
             "bool randomRotation ) const" );

}


std::map< std::string, gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > >
gkg::MeshFactory::getMeshes( const std::string& fileName ) const
{

  try
  {

    // reading the mesh configuration file
    gkg::BaseObjectReader baseObjectReader;
    std::ifstream is( fileName.c_str() );

    gkg::TypedObject< gkg::Dictionary > dictionary;
    baseObjectReader.read( is, dictionary );

    is.close();

    // instanciating the map of meshes
    std::map< std::string,
              gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > >
      meshes;

    // looping of the objects
    std::string name;
    std::string objectType;
    gkg::Dictionary::iterator o = dictionary.getTypedValue().begin(),
                              oe = dictionary.getTypedValue().end();

    while ( o != oe )
    {

      name = o->first;
      gkg::Dictionary& parameterDictionary = o->second->getValue<
                                                            gkg::Dictionary >();


      objectType = parameterDictionary[ "type" ]->getString();
      gkg::Dictionary& parameters = parameterDictionary[ "parameters" ]->
                                                  getValue< gkg::Dictionary >();

      if ( objectType == "cylinder" )
      {

        meshes[ name ] = getCylinder( parameters );

      }
      else if ( objectType == "generalized_cylinder" )
      {

        meshes[ name ] = getGeneralizedCylinder( parameters );

      }
      else if ( objectType == "fiber" )
      {

        meshes[ name ] = getFiber( parameters );

      }
      else if ( objectType == "ellipsoid" )
      {

        meshes[ name ] = getEllipsoid( parameters );

      }
      else if ( objectType == "sphere" )
      {

        meshes[ name ] = getSphere( parameters );

      }
      else if ( objectType == "star" )
      {

        meshes[ name ] = getStar( parameters );

      }
      else if ( objectType == "box" )
      {

        meshes[ name ] = getBox( parameters );

      }
      else if ( objectType == "network_of_cylinders" )
      {

        meshes[ name ] = getNetworkOfCylinders( parameters );

      }
      else if ( objectType == "network_of_generalized_cylinders" )
      {

        meshes[ name ] = getNetworkOfGeneralizedCylinders( parameters );

      }
      else if ( objectType == "network_of_fibers" )
      {

        meshes[ name ] = getNetworkOfFibers( parameters );

      }
      else if ( objectType == "network_of_ellipsoids" )
      {

        meshes[ name ] = getNetworkOfEllipsoids( parameters );

      }
      else if ( objectType == "network_of_spheres" )
      {

        meshes[ name ] = getNetworkOfSpheres( parameters );

      }
      else if ( objectType == "network_of_stars" )
      {

        meshes[ name ] = getNetworkOfStars( parameters );

      }
      else if ( objectType == "network_of_boxes" )
      {

        meshes[ name ] = getNetworkOfBoxes( parameters );

      }
      else
      {

        throw std::runtime_error( "Unknown object (mesh) type" );

      }

      ++ o;

    }

    return meshes;

  }
  GKG_CATCH( "std::map< std::string, "
             "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > > "
             "gkg::MeshFactory::getMeshes( "
             "const std::string& fileName ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getCylinder( gkg::Dictionary& parameters ) const
{

  try
  {

    return getCylinder( getVector3d( parameters[ "center" ] ),
                        getVector3d( parameters[ "axis" ] ),
                        getFloat( parameters[ "length" ] ),
                        getFloat( parameters[ "radius1" ] ),
                        getFloat( parameters[ "radius2" ] ),
                        getInteger( parameters[ "facet_count" ] ),
                        getBoolean( parameters[ "closed" ] ),
                        getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getCylinder( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getGeneralizedCylinder( gkg::Dictionary& parameters ) const
{

  try
  {

    return getGeneralizedCylinder(
                               getLightCurve3d( parameters[ "cross_section" ] ),
                               getFloat( parameters[ "length" ] ),
                               getBoolean( parameters[ "closed" ] ),
                               getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getGeneralizedCylinder( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getFiber( gkg::Dictionary& parameters ) const
{

  try
  {

    return getFiber( getLightCurve3d( parameters[ "points" ] ),
                     getVectorOfFloat( parameters[ "radii" ] ),
                     getInteger( parameters[ "vertex_count_per_plane" ] ),
                     getBoolean( parameters[ "random_rotation" ] ) );


  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getFiber( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getEllipsoid( gkg::Dictionary& parameters ) const
{

  try
  {

    return getEllipsoid( getVector3d( parameters[ "center" ] ),
                         getVector3d( parameters[ "eigenvector1" ] ),
                         getVector3d( parameters[ "eigenvector2" ] ),
                         getVector3d( parameters[ "eigenvector3" ] ),
                         getFloat( parameters[ "radius1" ] ),
                         getFloat( parameters[ "radius2" ] ),
                         getFloat( parameters[ "radius3" ] ),
                         getInteger( parameters[ "sphere_vertex_count" ] ),
                         getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getEllipsoid( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getSphere( gkg::Dictionary& parameters ) const
{

  try
  {

    return getSphere( getVector3d( parameters[ "center" ] ),
                      getFloat( parameters[ "radius" ] ),
                      getInteger( parameters[ "sphere_vertex_count" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getSphere( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getStar( gkg::Dictionary& parameters ) const
{

  try
  {

    // converting from degrees to radian
    std::vector< float > peakApertureAngles = 
            getVectorOfFloat( parameters[ "peak_aperture_angles_in_degrees" ] );
    std::vector< float >::iterator a = peakApertureAngles.begin(),
                                   ae = peakApertureAngles.end();
    while ( a != ae )
    {

      *a *= M_PI / 180.0;
      ++ a;

    }

    return getStar( getVector3d( parameters[ "center" ] ),
                    getFloat( parameters[ "radius" ] ),
                    getInteger( parameters[ "peak_count" ] ),
                    getVectorOfFloat( parameters[ "peak_magnitudes" ] ),
                    peakApertureAngles,
                    getInteger( parameters[ "sphere_vertex_count" ] ),
                    getBoolean( parameters[ "sharp_peak" ] ),
                    getBoolean( parameters[ "symmetric_peak" ] ),
                    getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getStar( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getBox( gkg::Dictionary& parameters ) const
{

  try
  {

    return getBox( getVector3d( parameters[ "lower_point" ] ),
                   getVector3d( parameters[ "upper_point" ] ),
                   getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getBox( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfCylinders( gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfCylinders(
                                getVector3d( parameters[ "center" ] ),
                                getVector3d( parameters[ "axis" ] ),
                                getFloat( parameters[ "length" ] ),
                                getFloat( parameters[ "radius1" ] ),
                                getFloat( parameters[ "radius2" ] ),
                                getInteger( parameters[ "facet_count" ] ),
                                getBoolean( parameters[ "closed" ] ),
                                getVector3d( parameters[ "network_axis1" ] ),
                                getVector3d( parameters[ "network_axis2" ] ),
                                getFloat( parameters[ "network_spacing1" ] ),
                                getFloat( parameters[ "network_spacing2" ] ),
                                getInteger( parameters[ "network_count1" ] ),
                                getInteger( parameters[ "network_count2" ] ),
                                getBoolean( parameters[ "random_rotation" ] ) );


  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfCylinders( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
gkg::MeshFactory::getNetworkOfGeneralizedCylinders(
                                             gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfGeneralizedCylinders(
                               getLightCurve3d( parameters[ "cross_section" ] ),
                               getFloat( parameters[ "length" ] ),
                               getBoolean( parameters[ "closed" ] ),
                               getVector3d( parameters[ "network_axis1" ] ),
                               getVector3d( parameters[ "network_axis2" ] ),
                               getFloat( parameters[ "network_spacing1" ] ),
                               getFloat( parameters[ "network_spacing2" ] ),
                               getInteger( parameters[ "network_count1" ] ),
                               getInteger( parameters[ "network_count2" ] ),
                               getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfGeneralizedCylinders( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfFibers( gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfFibers(
                           getLightCurve3d( parameters[ "points" ] ),
                           getVectorOfFloat( parameters[ "radii" ] ),
                           getInteger( parameters[ "vertex_count_per_plane" ] ),
                           getVector3d( parameters[ "network_axis1" ] ),
                           getVector3d( parameters[ "network_axis2" ] ),
                           getFloat( parameters[ "network_spacing1" ] ),
                           getFloat( parameters[ "network_spacing2" ] ),
                           getInteger( parameters[ "network_count1" ] ),
                           getInteger( parameters[ "network_count2" ] ),
                           getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfFibers( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfEllipsoids( gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfEllipsoids(
                             getVector3d( parameters[ "center" ] ),
                             getVector3d( parameters[ "eigenvector1" ] ),
                             getVector3d( parameters[ "eigenvector2" ] ),
                             getVector3d( parameters[ "eigenvector3" ] ),
                             getFloat( parameters[ "radius1" ] ),
                             getFloat( parameters[ "radius2" ] ),
                             getFloat( parameters[ "radius3" ] ),
                             getInteger( parameters[ "sphere_vertex_count" ] ),
                             getVector3d( parameters[ "network_axis1" ] ),
                             getVector3d( parameters[ "network_axis2" ] ),
                             getVector3d( parameters[ "network_axis3" ] ),
                             getFloat( parameters[ "network_spacing1" ] ),
                             getFloat( parameters[ "network_spacing2" ] ),
                             getFloat( parameters[ "network_spacing3" ] ),
                             getInteger( parameters[ "network_count1" ] ),
                             getInteger( parameters[ "network_count2" ] ),
                             getInteger( parameters[ "network_count3" ] ),
                             getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfEllipsoids( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfSpheres( gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfSpheres(
                              getVector3d( parameters[ "center" ] ),
                              getFloat( parameters[ "radius" ] ),
                              getInteger( parameters[ "sphere_vertex_count" ] ),
                              getVector3d( parameters[ "network_axis1" ] ),
                              getVector3d( parameters[ "network_axis2" ] ),
                              getVector3d( parameters[ "network_axis3" ] ),
                              getFloat( parameters[ "network_spacing1" ] ),
                              getFloat( parameters[ "network_spacing2" ] ),
                              getFloat( parameters[ "network_spacing3" ] ),
                              getInteger( parameters[ "network_count1" ] ),
                              getInteger( parameters[ "network_count2" ] ),
                              getInteger( parameters[ "network_count3" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfSpheres( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfStars( gkg::Dictionary& parameters ) const
{

  try
  {

    // converting from degrees to radian
    std::vector< float > peakApertureAngles = 
            getVectorOfFloat( parameters[ "peak_aperture_angles_in_degrees" ] );
    std::vector< float >::iterator a = peakApertureAngles.begin(),
                                   ae = peakApertureAngles.end();
    while ( a != ae )
    {

      *a *= M_PI / 180.0;
      ++ a;

    }

    return getNetworkOfStars(
                            getVector3d( parameters[ "center" ] ),
                            getFloat( parameters[ "radius" ] ),
                            getInteger( parameters[ "peak_count" ] ),
                            getVectorOfFloat( parameters[ "peak_magnitudes" ] ),
                            peakApertureAngles,
                            getInteger( parameters[ "sphere_vertex_count" ] ),
                            getBoolean( parameters[ "sharp_peak" ] ),
                            getBoolean( parameters[ "symmetric_peak" ] ),
                            getVector3d( parameters[ "network_axis1" ] ),
                            getVector3d( parameters[ "network_axis2" ] ),
                            getVector3d( parameters[ "network_axis3" ] ),
                            getFloat( parameters[ "network_spacing1" ] ),
                            getFloat( parameters[ "network_spacing2" ] ),
                            getFloat( parameters[ "network_spacing3" ] ),
                            getInteger( parameters[ "network_count1" ] ),
                            getInteger( parameters[ "network_count2" ] ),
                            getInteger( parameters[ "network_count3" ] ),
                            getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfStars( "
             "gkg::Dictionary& parameters ) const" );

}


gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > 
gkg::MeshFactory::getNetworkOfBoxes( gkg::Dictionary& parameters ) const
{

  try
  {

    return getNetworkOfBoxes( getVector3d( parameters[ "lower_point" ] ),
                              getVector3d( parameters[ "upper_point" ] ),
                              getVector3d( parameters[ "network_axis1" ] ),
                              getVector3d( parameters[ "network_axis2" ] ),
                              getVector3d( parameters[ "network_axis3" ] ),
                              getFloat( parameters[ "network_spacing1" ] ),
                              getFloat( parameters[ "network_spacing2" ] ),
                              getFloat( parameters[ "network_spacing3" ] ),
                              getInteger( parameters[ "network_count1" ] ),
                              getInteger( parameters[ "network_count2" ] ),
                              getInteger( parameters[ "network_count3" ] ),
                              getBoolean( parameters[ "random_rotation" ] ) );

  }
  GKG_CATCH( "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > "
             "gkg::MeshFactory::getNetworkOfBoxes( "
             "gkg::Dictionary& parameters ) const" );

}


std::vector< gkg::Vector3d< float > >
gkg::MeshFactory::getRotationAngles( int32_t angleCount ) const
{

  try
  {

    // preparing radomizer
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // allocating output vector
    std::vector< gkg::Vector3d< float > > rotationAngles( angleCount );

    int32_t a;
    for ( a = 0; a < angleCount; a++ )
    {

      rotationAngles[ a ].x = ( float )factory->getUniformRandomNumber(
                                                                randomGenerator,
                                                                0.0,
                                                                M_PI );
      rotationAngles[ a ].y = ( float )factory->getUniformRandomNumber(
                                                                randomGenerator,
                                                                0.0,
                                                                M_PI );
      rotationAngles[ a ].z = ( float )factory->getUniformRandomNumber(
                                                                randomGenerator,
                                                                0.0,
                                                                M_PI );

    }

    return rotationAngles;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< float > > "
             "Gkg::MeshFactory::getRotationAngles( "
             "int32_t angleCount ) const" );

}


gkg::Vector3d< float > 
gkg::MeshFactory::getVector3d( gkg::GenericObject& genericObject ) const
{

  try
  {

    gkg::GenericObjectList genericObjectList = genericObject->getValue<
                                                     gkg::GenericObjectList >();

    gkg::Vector3d< float > result( ( float )genericObjectList[ 0 ]->getScalar(),
                                   ( float )genericObjectList[ 1 ]->getScalar(),
                                   ( float )genericObjectList[ 2 ]->getScalar()
                                  );
    return result;

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::MeshFactory::getVector3d( "
             "gkg::GenericObject& genericObject ) const" );

}


float gkg::MeshFactory::getFloat( gkg::GenericObject& genericObject ) const
{

  try
  {

    float result = ( float )genericObject->getScalar();

    return result;

  }
  GKG_CATCH( "float gkg::MeshFactory::getFloat( "
             "gkg::GenericObject& genericObject ) const" );

}


int32_t gkg::MeshFactory::getInteger( gkg::GenericObject& genericObject ) const
{

  try
  {

    int32_t result = ( int32_t )( genericObject->getScalar() + 0.5 );

    return result;

  }
  GKG_CATCH( "int32_t gkg::MeshFactory::getInteger( "
             "gkg::GenericObject& genericObject ) const" );

}


bool gkg::MeshFactory::getBoolean( gkg::GenericObject& genericObject ) const
{

  try
  {

    bool result = ( ( int32_t )( genericObject->getScalar() + 0.5 ) ? true :
                                                                      false );

    return result;

  }
  GKG_CATCH( "bool gkg::MeshFactory::getBoolean( "
             "gkg::GenericObject& genericObject ) const" );

}


gkg::LightCurve3d< float > 
gkg::MeshFactory::getLightCurve3d( gkg::GenericObject& genericObject ) const
{

  try
  {

    gkg::GenericObjectList genericObjectList = genericObject->getValue<
                                                     gkg::GenericObjectList >();
    int32_t count = genericObjectList.size();
    std::vector< gkg::Vector3d< float > > vec( count );

    int32_t i = 0;
    for ( i = 0; i < count; i++ )
    {

      gkg::GenericObjectList
        tmp = genericObjectList[ i ]->getValue< gkg::GenericObjectList >();
      vec[ i ].x = ( float )tmp[ 0 ]->getScalar();
      vec[ i ].y = ( float )tmp[ 1 ]->getScalar();
      vec[ i ].z = ( float )tmp[ 2 ]->getScalar();

    }

    gkg::LightCurve3d< float >  result( vec );

    return result;

  }
  GKG_CATCH( "gkg::LightCurve3d< float > "
             "gkg::MeshFactory::getLightCurve3d( "
             "gkg::GenericObject& genericObject ) const" );

}


std::vector< float > 
gkg::MeshFactory::getVectorOfFloat( gkg::GenericObject& genericObject ) const
{

  try
  {

    gkg::GenericObjectList genericObjectList = genericObject->getValue<
                                                     gkg::GenericObjectList >();
    int32_t count = genericObjectList.size();
    std::vector< float > result( count );

    int32_t i = 0;
    for ( i = 0; i < count; i++ )
    {

      result[ i ] = ( float )genericObjectList[ i ]->getScalar();

    }

    return result;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::MeshFactory::getVectorOfFloat( "
             "gkg::GenericObject& genericObject ) const" );

}

