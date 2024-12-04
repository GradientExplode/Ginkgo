#include <gkg-processing-plugin-gsl-lapack/GSLRandomGeneratorImplementation.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>

gkg::GSLRandomGeneratorImplementation::GSLRandomGeneratorImplementation(
                                  gkg::RandomGenerator* randomGenerator,
                                  gkg::RandomGenerator::Type type )
                                      : gkg::RandomGeneratorImplementation(
                                                                randomGenerator,
                                                                type )
{

  try
  {

    ::gsl_rng_env_setup();
    switch ( type )
    {

      case gkg::RandomGenerator::Taus:

        _gslRandomGeneratorPointer = ::gsl_rng_alloc( ::gsl_rng_taus );
        break;

      case gkg::RandomGenerator::Gfsr4:

        _gslRandomGeneratorPointer = ::gsl_rng_alloc( ::gsl_rng_gfsr4 );
        break;

      case gkg::RandomGenerator::Mt19937:

        _gslRandomGeneratorPointer = ::gsl_rng_alloc( ::gsl_rng_mt19937 );
        break;

    }

  }
  GKG_CATCH( "gkg::GSLRandomGeneratorImplementation::"
             "GSLRandomGeneratorImplementation( "
             "gkg::RandomGenerator* randomGenerator, "
             "gkg::RandomGenerator::Type type )" );

}


gkg::GSLRandomGeneratorImplementation::~GSLRandomGeneratorImplementation()
{

  gsl_rng_free( _gslRandomGeneratorPointer );

}


void gkg::GSLRandomGeneratorImplementation::setSeed( uint32_t s )
{

  try
  {

    ::gsl_rng_set( _gslRandomGeneratorPointer, s );

  }
  GKG_CATCH( "void gkg::GSLRandomGeneratorImplementation::setSeed( "
             "uint32_t s )" );


}


gsl_rng* 
gkg::GSLRandomGeneratorImplementation::getGSLRandomGeneratorPointer() const
{

  return _gslRandomGeneratorPointer;

}


gkg::GSLRandomGeneratorImplementation* 
gkg::GSLRandomGeneratorImplementation::getImplementation(
                                         gkg::RandomGenerator* randomGenerator )
{

  return static_cast< gkg::GSLRandomGeneratorImplementation* >(
                                         randomGenerator->getImplementation() );

}

