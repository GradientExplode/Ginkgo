#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>
#include <iostream>
#include <fstream>
#include <string>


FORCE_HARD_LINK_WITH_CONNECTOMIST


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    //
    // creating first bundle
    //
    gkg::BundleMap< std::string >::Bundle bundle1;

    // creating 3D curves
    gkg::Curve3d< float > curve1;
    curve1.addPoint( gkg::Vector3d< float >( 0, 0, 0 ) );
    curve1.addPoint( gkg::Vector3d< float >( 1, 0, 0 ) );
    curve1.addPoint( gkg::Vector3d< float >( 2, 0, 0 ) );
    bundle1.addCurve3d( curve1 );

    gkg::Curve3d< float > curve2;
    curve2.addPoint( gkg::Vector3d< float >( 0, 0, 0 ) );
    curve2.addPoint( gkg::Vector3d< float >( 0, 1, 0 ) );
    curve2.addPoint( gkg::Vector3d< float >( 0, 2, 0 ) );
    bundle1.addCurve3d( curve2 );


    //
    // creating second bundle
    //
    gkg::BundleMap< std::string >::Bundle bundle2;

    // creating 3D curves
    gkg::Curve3d< float > curve3;
    curve3.addPoint( gkg::Vector3d< float >( 0, 0, 0 ) );
    curve3.addPoint( gkg::Vector3d< float >( 2, 0, 0 ) );
    curve3.addPoint( gkg::Vector3d< float >( 4, 0, 0 ) );
    bundle2.addCurve3d( curve3 );

    gkg::Curve3d< float > curve4;
    curve4.addPoint( gkg::Vector3d< float >( 0, 0, 0 ) );
    curve4.addPoint( gkg::Vector3d< float >( 0, 2, 0 ) );
    curve4.addPoint( gkg::Vector3d< float >( 0, 4, 0 ) );
    bundle2.addCurve3d( curve4 );

    gkg::Curve3d< float > curve5;
    curve5.addPoint( gkg::Vector3d< float >( 0, 0, 0 ) );
    curve5.addPoint( gkg::Vector3d< float >( 0, 0, 2 ) );
    curve5.addPoint( gkg::Vector3d< float >( 0, 0, 4 ) );
    bundle2.addCurve3d( curve5 );


    //
    // creating bundle map
    //
    gkg::BundleMap< std::string > bundleMap;
    bundleMap.addBundle( "first_bundle", bundle1 );
    bundleMap.addBundle( "second_bundle", bundle2 );


    //
    // displaying bundle map
    //
    gkg::BundleMap< std::string >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      std::cout << b->first << " : " << std::endl
                << b->second << std::endl;
      ++ b;

    }


    //
    // writing bundle map with PKT BundleMap format
    //
    gkg::Writer::getInstance().write( "foo", bundleMap, true,
                                      "bundlemap" );


    //
    // writing bundle map with Aims BundleMap format
    //
    gkg::Writer::getInstance().write( "aims", bundleMap, true,
                                      "aimsbundlemap" );


    //
    // reading bundle map with PKT BundleMap format
    //
    gkg::BundleMap< std::string > bundleMapBis;
    gkg::Reader::getInstance().read( "foo", bundleMapBis );


    //
    // displaying bundle map bis
    //
    b = bundleMapBis.begin();
    be = bundleMapBis.end();
    while ( b != be )
    {

      std::cout << b->first << " : " << std::endl
                << b->second << std::endl;
      ++ b;

    }


    //
    // reading bundle map with Aims BundleMap format
    //
    gkg::BundleMap< std::string > bundleMapTer;
    gkg::Reader::getInstance().read( "aims", bundleMapTer );


    //
    // displaying bundle map ter
    //
    b = bundleMapTer.begin();
    be = bundleMapTer.end();
    while ( b != be )
    {

      std::cout << b->first << " : " << std::endl
                << b->second << std::endl;
      ++ b;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
