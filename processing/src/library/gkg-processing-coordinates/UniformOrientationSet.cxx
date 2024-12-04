#include <gkg-processing-coordinates/UniformOrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>


gkg::UniformOrientationSet::UniformOrientationSet( int32_t count )
                           : gkg::OrientationSet( count )
{

  try
  {

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // looping over orientation(s)
    int32_t index;
    for ( index = 0; index < count; index++ )
    {

      _orientations[ index ].x = ( float )factory->getGaussianRandomNumber( 
                                                             randomGenerator, 
                                                             0.0, 1.0e3 ) / 
                                                                    1.0e3;
      _orientations[ index ].y = ( float )factory->getGaussianRandomNumber( 
                                                             randomGenerator, 
                                                             0.0, 1.0e3 ) / 
                                                                    1.0e3;
      _orientations[ index ].z = ( float )factory->getGaussianRandomNumber( 
                                                             randomGenerator, 
                                                             0.0, 1.0e3 ) / 
                                                                    1.0e3;
      _orientations[ index ].normalize();

    }

  }
  GKG_CATCH( "gkg::UniformOrientationSet::UniformOrientationSet( "
             "int32_t count )" );

}


gkg::UniformOrientationSet::~UniformOrientationSet()
{
}


