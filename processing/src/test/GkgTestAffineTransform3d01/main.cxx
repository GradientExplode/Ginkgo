#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-core-exception/Exception.h>
#include <cstdlib>
#include <fstream>
#include <iostream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::vector< float > scalingParameters;
    std::vector< float > shearingParameters;
    std::vector< float > rotationParameters;
    std::vector< float > translationParameters;
    std::string transformName;

    gkg::Application application( argc, argv,
                                  "Affine 3D transform generator" );
    application.addSeriesOption( "-s",
                                 "Scaling parameters",
                                 scalingParameters,
                                 3, 3 );
    application.addSeriesOption( "-sh",
                                 "Shearing parameters",
                                 shearingParameters,
                                 3, 3 );
    application.addSeriesOption( "-r",
                                 "Rotation parameters in degrees",
                                 rotationParameters,
                                 3, 3 );
    application.addSeriesOption( "-t",
                                 "Translation parameters in millimeters",
                                 translationParameters,
                                 3, 3 );
    application.addSingleOption( "-o",
                                 "Output homogeneous transform file name",
                                 transformName );

    application.initialize();


    gkg::AffineTransform3d< float >
      transform3d( scalingParameters[ 0 ],
                   scalingParameters[ 1 ],
                   scalingParameters[ 2 ],
                   shearingParameters[ 0 ],
                   shearingParameters[ 1 ],
                   shearingParameters[ 2 ],
                   rotationParameters[ 0 ],
                   rotationParameters[ 1 ],
                   rotationParameters[ 2 ],
                   translationParameters[ 0 ],
                   translationParameters[ 1 ],
                   translationParameters[ 2 ],
                   true );

    std::ofstream os( transformName.c_str() );
    transform3d.writeTrm( os );
    os.close();

    std::ifstream is( transformName.c_str() );
    gkg::AffineTransform3d< float > transform3dBis;
    transform3dBis.readTrm( is );

    gkg::Vector3d< float > scalings;
    gkg::Vector3d< float > shearings;
    gkg::Vector3d< float > angles;
    gkg::Vector3d< float > translation;

    transform3dBis.getDirectParameters( scalings, shearings,
                                        angles, translation, true );

    std::cout << "scalings    : " << scalings << std::endl;
    std::cout << "shearings   : " << shearings << std::endl;
    std::cout << "angles      : " << angles << std::endl;
    std::cout << "translation : " << translation << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
