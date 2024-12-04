#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


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
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );


for ( int32_t t = 0; t < 10; t++ )
{

    float u = factory->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );
    float v = factory->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );
    float w = factory->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );

    float norm = std::sqrt( u * u + v * v + w * w );

    u /= norm;
    v /= norm;
    w /= norm;

    float delta = 0.0; 
    float y = 0.0;
    do
    {

      y = factory->getUniformRandomNumber( randomGenerator, -1.0, 1.0 );

      delta = ( 2 * u * v * y ) * ( 2 * u * v * y ) -
                          4 * ( u * u + w * w ) *
                          ( ( v * v + w * w ) * y * y - w * w );
    }
    while ( delta < 0 );

 
    std::cout << "y: " << y <<std::endl;
    std::cout << "delta: " << delta <<std::endl;


    float x1 = -0.5 + ( 1.0 / ( 4 * u * v * y ) ) *
               std::sqrt( delta );

    float x2 = -0.5 - ( 1.0 / ( 4 * u * v * y ) ) *
               std::sqrt( delta );

    float z1 = - u * x1 / w - v * y / w;
    float z2 = - u * x2 / w - v * y / w;

    gkg::Vector3d< float > e1( u, v, w );
    gkg::Vector3d< float > e21( x1, y, z1 );
    gkg::Vector3d< float > e22( x2, y, z2 );

    e21.normalize();
    e22.normalize();

    gkg::Vector3d< float > e2;
    if ( e1.dot( e21 ) != 0 )
    {

      e2 = e22;

    }
    else
    {

      e2 = e21;

    }

    gkg::Vector3d< float > e3 = e1.cross( e2 );

    std::cout << "e1 : " << e1 << std::endl;
    std::cout << "e2 : " << e2 << std::endl;
    std::cout << "e3 : " << e3 << std::endl;

    std::cout << "e1.e2 = " << e1.dot( e2 ) << std::endl;
    std::cout << "e2.e3 = " << e2.dot( e3 ) << std::endl;

}

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
