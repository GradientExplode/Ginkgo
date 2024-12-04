#include <gkg-communication-plugin/PluginLoader.h>

#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>

#include <gkg-processing-container/Volume_i.h>

#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>



////////////////////////////////////////////////////////////////////////////////
// main function
////////////////////////////////////////////////////////////////////////////////

int main( int32_t /*argc*/, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    double a = 0.1;
    double b = 0.9;


    gkg::PluginLoader::getInstance().load();
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    int32_t outputOrientationCount = 400;
    gkg::OrientationSet* outputOrientationSet = 0;
    outputOrientationSet = new gkg::ElectrostaticOrientationSet( 
                                                       outputOrientationCount );
                               

    gkg::Volume< short > t2( 3, 1, 1 );
    t2.fill( 10000 );

    gkg::Volume< short > mask( 3, 1, 1 );
    mask.fill( 32767 );

    gkg::Volume< int16_t > dw( 3, 1, 1, outputOrientationCount );

    gkg::GenericObjectList orientations( outputOrientationCount );
    int32_t o;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      gkg::GenericObjectList orientation( 3 );
      orientation[ 0 ] = outputOrientationSet->getOrientation( o ).x;
      orientation[ 1 ] = outputOrientationSet->getOrientation( o ).y; 
      orientation[ 2 ] = outputOrientationSet->getOrientation( o ).z;
      orientations[ o ] = orientation;


    }
    std::vector< double > bValues( outputOrientationCount, 3000 );
    dw.getHeader().addAttribute( "bvalues", bValues );
    dw.getHeader().addAttribute( "diffusion_gradient_orientations",
                                            orientations );

    for ( o = 0; o < outputOrientationCount; o++ )
    {


       gkg::Vector3d< float > ori = outputOrientationSet->getOrientation( o );
       double radial, azimuth, colatitude;
       factory->getCartesianToSphericalCoordinates( ori.x,
                                                    ori.y,
                                                    ori.z,
                                                    radial,
                                                    azimuth,
                                                    colatitude );
       double x = a * std::sin( colatitude ) * std::cos( azimuth );
       double y = a * std::sin( colatitude ) * std::sin( azimuth );
       double z = b * std::cos( colatitude );
       double mag = std::sqrt( x * x + y * y + z * z );
       dw( 0, 0, 0, o ) = ( int16_t )( mag * 1000.0 );
       x = b * std::sin( colatitude ) * std::cos( azimuth );
       y = a * std::sin( colatitude ) * std::sin( azimuth );
       z = a * std::cos( colatitude );
       mag = std::sqrt( x * x + y * y + z * z );
       dw( 1, 0, 0, o ) = ( int16_t )( mag * 1000.0 );
       x = a * std::sin( colatitude ) * std::cos( azimuth );
       y = b * std::sin( colatitude ) * std::sin( azimuth );
       z = a * std::cos( colatitude );
       mag = std::sqrt( x * x + y * y + z * z );
       dw( 2, 0, 0, o ) = ( int16_t )( mag * 1000.0 );

    }


    gkg::Writer::getInstance().write( "t2.ima", t2 );
    gkg::Writer::getInstance().write( "dw.ima", dw );
    gkg::Writer::getInstance().write( "mask.ima", mask );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
