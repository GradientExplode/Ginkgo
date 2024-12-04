#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::OrientationSet*
      orientationSet = new gkg::ElectrostaticOrientationSet( 50 );

    gkg::MeshMap< int32_t, float, 3U > meshMap;
    gkg::Translation3d< float > translation;

    translation.setDirectTranslation( 0.0, 0.0, 0.0 );
    orientationSet->addSymmetricalMesh( translation, 0, meshMap );

    translation.setDirectTranslation( 5.0, 0.0, 5.0 );
    orientationSet->addSymmetricalMesh( translation, 0, meshMap );

    translation.setDirectTranslation( 0.0, -5.0, 0.0 );
    orientationSet->addSymmetricalMesh( translation, 1, meshMap );

    gkg::Writer::getInstance().write( "GkgTestOrientationSet01",
                                      meshMap, true, "aimsmesh" );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
