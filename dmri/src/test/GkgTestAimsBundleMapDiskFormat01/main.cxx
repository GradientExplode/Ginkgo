#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>


#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Writer_i.h>


FORCE_HARD_LINK_WITH_CONNECTOMIST


inline void printMemory( const std::string& text )
{


  std::cout << "\n----- " << text
            << " -------------------------------" << std::endl;
  std::cout << "process memory usage = "
            << gkg::SystemInfo::getInstance().getProcessMemoryUsage()
            << std::endl;

}


////////////////////////////////////////////////////////////////////////////////
// main function
////////////////////////////////////////////////////////////////////////////////

int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    std::vector< std::string > fileNameInputBundleMaps;

    gkg::Application application( argc, argv, "Test AimsBundleMapDiskFormat" );
    application.addSeriesOption( "-i",
                                 "Input bundle map file names",
                                 fileNameInputBundleMaps,
                                 1 );
    application.initialize();

    gkg::RCPointer< gkg::BundleMap< std::string > > 
      outputBundleMap( new gkg::BundleMap< std::string > );

    std::vector< std::string >::const_iterator
      f = fileNameInputBundleMaps.begin(),
      fe = fileNameInputBundleMaps.end();
    while ( f != fe )
    {

      std::cout << "========================================================="
                << std::endl;
      std::cout << "reading ' " << *f << "' input bundle map : " << std::endl;

      gkg::RCPointer< gkg::BundleMap< std::string > > 
        inputBundleMap( new gkg::BundleMap< std::string > );
      gkg::Reader::getInstance().read( *f, *inputBundleMap );


      std::cout << "========================================================="
                << std::endl;
      std::cout << "adding contribution of ' " << *f << std::endl;

      printMemory( std::string( "beginning of adding contribution " ) );

      gkg::BundleMap< std::string >::const_iterator
        ib = inputBundleMap->begin(),
        ibe = inputBundleMap->end();
      while ( ib != ibe )
      {

        outputBundleMap->addCurve3ds( ib->first, ib->second.getCurve3ds() );
        printMemory( std::string( "adding new bundle " ) );
        ++ ib;

      }

      inputBundleMap.reset();
      printMemory( std::string( "after resetting the input bundle map" ) );

      ++ f;

    }
/*

    int32_t curve3dCount = 1000;
    int32_t pointCount = 1000;
    int32_t c, p;

    std::cout << "memory of points = " << curve3dCount * pointCount * 3 * 
                 sizeof( float ) << std::endl;


    gkg::BundleMap< std::string > object;

    int32_t count = 0;
    for ( count = 0; count < 1; count++ )
    {

      std::cout << "==========================================================="
                << std::flush;
      printMemory( std::string( "start of bundle read " ) );

      std::vector< gkg::LightCurve3d< float > >*
        curve3ds = new std::vector< gkg::LightCurve3d< float > >( curve3dCount );

      printMemory( std::string( "after vector of curve3d allocation " ) );

      for ( c = 0; c < curve3dCount; c++ )
      {

        std::vector< gkg::Vector3d< float > > points( pointCount );
        for ( p = 0; p < pointCount; p++ )
        {

          points[ p ].x = ( float )p;
          points[ p ].y = ( float )p;
          points[ p ].z = ( float )p;

        }
        ( *curve3ds )[ c ].reserve( pointCount );
        ( *curve3ds )[ c ].addPoints( points );

      }

      printMemory( std::string( "after filling curve3ds with points " ) );

      gkg::BundleMap< std::string >::Bundle*
        bundle = new gkg::BundleMap< std::string >::Bundle( *curve3ds );

      printMemory( std::string( "after creating and filling bundle " ) );

      delete curve3ds;

      printMemory( std::string( "after deleting curve3ds " ) );

      object.addBundle( gkg::StringConverter::toString( count ), *bundle );

      printMemory( std::string( "after adding bundle to bundlemap " ) );

      delete bundle;
      printMemory( std::string( "after deleting bundle " ) );

    }

    gkg::Writer::getInstance().write( "test1.bundles", object );
*/
  }
  GKG_CATCH_COMMAND( result );

  return result;

}
