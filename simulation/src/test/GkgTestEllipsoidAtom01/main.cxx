#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


void run_test(const gkg::Vector3d<float>& Acenter, 
              const gkg::Vector3d<float>& Aaxis1,
              const gkg::Vector3d<float>& Aaxis2,
              const gkg::Vector3d<float>& Aaxis3 );

int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  { 

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // getting numerical analysis implementation factory
    //gkg::NumericalAnalysisImplementationFactory*
    //  factory = gkg::NumericalAnalysisSelector::getInstance().
    //                                                 getImplementationFactory();

    gkg::Vector3d< float > center( 5.0, -1.22, 0.45 );
    float radius1 = 2.0;
    float radius2 = 0.5;
    float radius3 = 1.2;

    gkg::Vector3d< float > axis1( 1.0, 0.0, 0.0 );
    gkg::Vector3d< float > axis2( 0.0, 1.0, 0.0 );
    gkg::Vector3d< float > axis3( 0.0, 0.0, 1.0 );


    gkg::EllipsoidAtom E1( center,
                           radius1, radius1, radius3,
                           axis1, axis2, axis3 );


    std::cout << "ellipsoid 1 :" << std::endl;
    std::cout << "===========" << std::endl;
    std::cout << "center = " << center << std::endl;
    std::cout << "radius1 = " << radius1 << std::endl;
    std::cout << "radius2 = " << radius2 << std::endl;
    std::cout << "radius3 = " << radius3 << std::endl;
    std::cout << "axis1 = " << axis1 << std::endl;
    std::cout << "axis2 = " << axis2 << std::endl;
    std::cout << "axis3 = " << axis3 << std::endl;
    std::cout << "vector representation = " << std::endl;
    int32_t p = 0;
    for ( p = 0; p < 10; p++ )
    {

      std::cout << "    " << E1.parameters[ p ] << std::endl;;

    }



    gkg::EllipsoidAtom E2( E1 );

    std::cout << "ellipsoid 2 :" << std::endl;
    std::cout << "===========" << std::endl;

    std::cout << "vector representation = " << std::endl;
    for ( p = 0; p < 10; p++ )
    {

      std::cout << "    " << E2.parameters[ p ] << std::endl;;

    }
                    
    E2.getStdParameters( center,
                         radius1, radius1, radius3,
                         axis1, axis2, axis3 );

    std::cout << "center = " << center << std::endl;
    std::cout << "radius1 = " << radius1 << std::endl;
    std::cout << "radius2 = " << radius2 << std::endl;
    std::cout << "radius3 = " << radius3 << std::endl;
    std::cout << "axis1 = " << axis1 << std::endl;
    std::cout << "axis2 = " << axis2 << std::endl;
    std::cout << "axis3 = " << axis3 << std::endl;

      
  }

  GKG_CATCH_COMMAND( result );

  return result;

}

