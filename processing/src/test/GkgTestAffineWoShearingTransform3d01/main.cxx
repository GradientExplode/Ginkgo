#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <cstdlib>
#include <fstream>
#include <iostream>


int main()
{

  gkg::PluginLoader::getInstance().load();

  // in fact, 'gsl' library is the default; so the following line is not
  // mandatory: it is only for didactic purpose
  gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

  gkg::AffineWoShearingTransform3d< float >
    transform3d( 2, 3, 4, 10, 20, 30, 20, 25, 30, true );

  std::ofstream os( "affine.trm" );
  transform3d.writeTrm( os );
  os.close();

  std::ifstream is( "affine.trm" );
  gkg::AffineWoShearingTransform3d< float > transform3dBis;
  transform3dBis.readTrm( is );

  gkg::Vector3d< float > scalings;
  gkg::Vector3d< float > angles;
  gkg::Vector3d< float > translation;

  transform3dBis.getDirectParameters( scalings, angles, translation, true );

  std::cout << "scalings    (should be 2,3,4)    : "
            << scalings << std::endl;
  std::cout << "angles      (should be 10,20,30) : "
            << angles << std::endl;
  std::cout << "translation (should be 20,25,30) : "
            << translation << std::endl;

  return EXIT_SUCCESS;

}
