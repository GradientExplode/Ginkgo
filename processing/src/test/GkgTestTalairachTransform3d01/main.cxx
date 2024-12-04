#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/TalairachTransform3d_i.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <fstream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameAnyVolume;
    std::string fileNameTalairachVolume;

    gkg::Application application( argc, argv,
                                  "Test for TalairachTransform3d class" );
    application.addSingleOption( "-anyVolume",
                                 "Any volume",
                                 fileNameAnyVolume );
    application.addSingleOption( "-talairachVolume",
                                 "Talairach volume",
                                 fileNameTalairachVolume );
    application.initialize();


    gkg::Volume< int16_t > anyVolume;
    gkg::Volume< int16_t > talairachVolume;


    gkg::Reader::getInstance().read( fileNameAnyVolume, anyVolume );
    gkg::Reader::getInstance().read( fileNameTalairachVolume, talairachVolume );

    gkg::TalairachTransform3d< float >
      talairachTransform3d( anyVolume, talairachVolume );

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << talairachTransform3d.getComposition() << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    int32_t i = 0;
    int32_t j = 0;
    gkg::Matrix rotationAndScaling( 3, 3 );
    for ( i = 0; i < 3; i++ )
    {

      for ( j = 0; j < 3; j++ )
      {

         rotationAndScaling( i, j ) = talairachTransform3d.getComposition().
                                                   getDirectCoefficient( i, j );

      }

    }
    std::cout << rotationAndScaling << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;

    gkg::Vector3d< float > translations;
    translations.x = talairachTransform3d.getComposition().
                                                   getDirectCoefficient( 0, 3 );
    translations.y = talairachTransform3d.getComposition().
                                                   getDirectCoefficient( 1, 3 );
    translations.z = talairachTransform3d.getComposition().
                                                   getDirectCoefficient( 2, 3 );
    std::cout << "translations = " << translations << std::endl;


    gkg::Vector3d< float > scalings;
    gkg::Vector3d< float > u;
    gkg::Vector3d< float > v;
    gkg::Vector3d< float > w;
    gkg::getXYZScalingsAndTrieder( rotationAndScaling, scalings, u, v, w );

    std::cout << "scalings = " << scalings << std::endl;
    std::cout << "u = " << u << std::endl;
    std::cout << "v = " << v << std::endl;
    std::cout << "w = " << w << std::endl;
    std::cout << "norm(u) = " << u.getNorm() << std::endl;
    std::cout << "norm(v) = " << v.getNorm() << std::endl;
    std::cout << "norm(w) = " << w.getNorm() << std::endl;
    std::cout << "u.dot(v) = " << u.dot( v ) << std::endl;
    std::cout << "u.dot(w) = " << u.dot( w ) << std::endl;
    std::cout << "v.dot(w) = " << v.dot( w ) << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;

    gkg::Vector3d< float > angles = gkg::getXYZAnglesFromTrieder( u, v, w );

    std::cout << "angles = "
              << angles
              << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    gkg::AffineWoShearingTransform3d< float > 
      affineWoShearingTransform3d( scalings, angles, translations );

    for ( i = 0; i < 4; i++ )
    {

      for ( j = 0; j < 4; j++ )
      {

         std::cout << affineWoShearingTransform3d.getComposition().
                        getDirectCoefficient( i, j ) << "  ";

      }
      std::cout << std::endl;

    }

    std::cout << "-----------------------------------------------" << std::endl;
    gkg::Vector3d< float > from , to;

    from.x = 1;
    from.y = 2;
    from.z = 4;

    affineWoShearingTransform3d.getDirect( from, to );
    std::cout << "to(1) = " << to << std::endl;

    talairachTransform3d.getDirect( from, to );
    std::cout << "to(2) = " << to << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    from.x = 200;
    from.y = 100;
    from.z = 60;

    affineWoShearingTransform3d.getDirect( from, to );
    std::cout << "to(1) = " << to << std::endl;

    talairachTransform3d.getDirect( from, to );
    std::cout << "to(2) = " << to << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

