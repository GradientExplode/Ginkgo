#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <list>


#define FIBER_COUNT 10
#define POINT_COUNT 50


////////////////////////////////////////////////////////////////////////////////
// main function
////////////////////////////////////////////////////////////////////////////////

int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {


    int32_t type = 0;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, "DWI connectivity matrix tool" );
    application.addSingleOption( "-type",
                                 "Type of simulation:\n"
                                 "- 1 -> 3 perpendicular fiber bundles\n"
                                 "- 2 -> 3 perpendicular fibers",
                                 type );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // building spherical mesh
    ////////////////////////////////////////////////////////////////////////////

    gkg::ElectrostaticOrientationSet orientationSet( 8000, false );
    gkg::MeshMap< int32_t, float, 3U > orientationMeshMap;
    gkg::ConvexHull::getInstance().addConvexHull(
                                             orientationSet.getOrientations(),
                                             0,
                                             orientationMeshMap );

    int32_t orientationVertexCount = 
      orientationMeshMap.vertices.getSiteCount( 0 );

    gkg::MeshScaler< int32_t, float, 3U > meshScaler;
    std::vector< float > scaling( orientationVertexCount, 100.0 );
    meshScaler.scale( orientationMeshMap, 0, scaling, orientationMeshMap );

    gkg::Writer::getInstance().write( "sphere",
                                      orientationMeshMap,
                                      false,
                                      "aimsmesh" );


    ////////////////////////////////////////////////////////////////////////////
    // building bundle map
    ////////////////////////////////////////////////////////////////////////////

    gkg::BundleMap< std::string > bundleMap;

    std::list< gkg::BundleMap< std::string >::Fiber > fibersAlongX;
    std::list< gkg::BundleMap< std::string >::Fiber > fibersAlongY;
    std::list< gkg::BundleMap< std::string >::Fiber > fibersAlongZ;

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t p = 0;

    // fibers along z
    if ( type == 1 )
    {

      for ( y = 0; y < FIBER_COUNT; y++ )
      {

        for ( x = 0; x < FIBER_COUNT; x++ )
        {

          std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
          gkg::LightCurve3d< float > fiber;
          for ( p = 0; p < POINT_COUNT + 1; p++ )
          {

            points[ p ] = gkg::Vector3d< float >( 2.0 * ( -5.0 + ( float )x ),
                                                  2.0 * ( -5.0 + ( float )y ),
                                                  -100.0 + ( float )p * 4.0 );
          }
          fiber.addPoints( points );
          fibersAlongZ.push_back( fiber );

        }

      }

    }
    else if ( type == 2 )
    {

      std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
      gkg::LightCurve3d< float > fiber;
      for ( p = 0; p < POINT_COUNT + 1; p++ )
      {

        points[ p ] = gkg::Vector3d< float >( 0.0,
                                              0.0,
                                              -100.0 + ( float )p * 4.0 );
      }
      fiber.addPoints( points );
      fibersAlongZ.push_back( fiber );

    }
    bundleMap.addCurve3ds( "Whole-brain", fibersAlongZ );

    // fibers along y
    if ( type == 1 )
    {

      for ( z = 0; z < FIBER_COUNT; z++ )
      {

        for ( x = 0; x < FIBER_COUNT; x++ )
        {

          std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
          gkg::LightCurve3d< float > fiber;
          for ( p = 0; p < POINT_COUNT + 1 ; p++ )
          {

            points[ p ] = gkg::Vector3d< float >( 2.0 * ( -5.0 + ( float )x ),
                                                  -100.0 + ( float )p * 4.0,
                                                  2.0 * ( -5.0 + ( float )z ) );
          }
          fiber.addPoints( points );
          fibersAlongY.push_back( fiber );

        }

      }

    }
    else if ( type == 2 )
    {

      std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
      gkg::LightCurve3d< float > fiber;
      for ( p = 0; p < POINT_COUNT + 1; p++ )
      {

        points[ p ] = gkg::Vector3d< float >( 0.0,
                                              -100.0 + ( float )p * 4.0,
                                              0.0 );
      }
      fiber.addPoints( points );
      fibersAlongY.push_back( fiber );

    }
    bundleMap.addCurve3ds( "Whole-brain", fibersAlongY );

    // fibers along x
    if ( type == 1 )
    {

      for ( z = 0; z < FIBER_COUNT; z++ )
      {

        for ( y = 0; y < FIBER_COUNT; y++ )
        {

          std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
          gkg::LightCurve3d< float > fiber;
          for ( p = 0; p < POINT_COUNT + 1; p++ )
          {

            points[ p ] = gkg::Vector3d< float >( -100.0 + ( float )p * 4.0,
                                                  2.0 * ( -5.0 + ( float )y ),
                                                  2.0 * ( -5.0 + ( float )z ) );

          }
          fiber.addPoints( points );
          fibersAlongX.push_back( fiber );

        }

      }

    }
    else if ( type == 2 )
    {

      std::vector< gkg::Vector3d< float > > points( POINT_COUNT + 1 );
      gkg::LightCurve3d< float > fiber;
      for ( p = 0; p < POINT_COUNT + 1; p++ )
      {

        points[ p ] = gkg::Vector3d< float >( -100.0 + ( float )p * 4.0,
                                              0.0,
                                              0.0 );
      }
      fiber.addPoints( points );
      fibersAlongX.push_back( fiber );

    }
    bundleMap.addCurve3ds( "Whole-brain", fibersAlongX );


    gkg::Writer::getInstance().write( "fibers",
                                      bundleMap,
                                      false,
                                      "aimsbundlemap" );


    ////////////////////////////////////////////////////////////////////////////
    // building label volume
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< int16_t > labels( 100, 100, 100 );
    labels.getHeader().addAttribute( "resolutionX", 2.0 );
    labels.getHeader().addAttribute( "resolutionY", 2.0 );
    labels.getHeader().addAttribute( "resolutionZ", 2.0 );

    labels.fill( 0 );

    for ( z = 0; z < FIBER_COUNT; z++ )
    {

      for ( y = 0; y < FIBER_COUNT; y++ )
      {

        for ( x = 0; x < FIBER_COUNT; x++ )
        {

          labels( 50 - FIBER_COUNT / 2 + x,
                  50 - FIBER_COUNT / 2 + y,
                  50 - FIBER_COUNT / 2 + z ) = 3;
                  
        }

      }

    }

    gkg::Writer::getInstance().write( "labels",
                                      labels );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
