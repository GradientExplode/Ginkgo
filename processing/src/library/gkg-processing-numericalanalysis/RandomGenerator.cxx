#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/RandomGeneratorImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <sys/time.h>


gkg::RandomGenerator::RandomGenerator( gkg::RandomGenerator::Type type )
{

  try
  {

    _randomGeneratorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                              createRandomGeneratorImplementation( this, type );

  }
  GKG_CATCH( "gkg::RandomGenerator::RandomGenerator( "
             "gkg::RandomGenerator::Type type )" );

}


gkg::RandomGenerator::~RandomGenerator()
{

  delete _randomGeneratorImplementation;

}


void gkg::RandomGenerator::setSeed( uint32_t s )
{

  try
  {

    _randomGeneratorImplementation->setSeed( s );

  }
  GKG_CATCH( "void gkg::RandomGenerator::setSeed( uint32_t s )" );


}


void gkg::RandomGenerator::setSeedWithTime()
{

  try
  {

    timeval tv;
    gettimeofday( &tv, 0 );

    _randomGeneratorImplementation->setSeed( 1000000U * tv.tv_sec +
                                             tv.tv_usec );

  }
  GKG_CATCH( "void gkg::RandomGenerator::setSeedWithTime()" );


}


gkg::RandomGeneratorImplementation* 
gkg::RandomGenerator::getImplementation() const
{

  return _randomGeneratorImplementation;

}
