#include <gkg-simulation-distribution/GammaDistribution.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::GammaDistribution::GammaDistribution( const gkg::Dictionary& parameters )
                       : gkg::ParameterDistribution()
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( "mean" );

    // in case mean and stddev parameters are provided
    if ( p != parameters.end() )
    {

      float mean = ( float )p->second->getScalar();

      p = parameters.find( "stddev" );
      if ( p == parameters.end() )
      {

        throw std::runtime_error( "a 'stddev' item is required" );

      }
      float stddev = ( float )p->second->getScalar();

      _scale = ( stddev * stddev ) / mean;
      _shape = ( mean * mean ) / ( stddev * stddev );

      p = parameters.find( "shape" );
      if ( p != parameters.end() )
      {

        throw std::runtime_error(
                   "'shape' item incompatible with 'mean' and 'stddev' items" );

      }

      p = parameters.find( "scale" );
      if ( p != parameters.end() )
      {

        throw std::runtime_error(
                   "'scale' item incompatible with 'mean' and 'stddev' items" );

      }

    }
    // in case scale and shape parameters are provided
    else
    {

      p = parameters.find( "scale" );
      if ( p == parameters.end() )
      {

        throw std::runtime_error( "a 'scale' or 'mean' item is required" );

      }
      _scale = ( float )p->second->getScalar();

      p = parameters.find( "shape" );
      if ( p == parameters.end() )
      {

        throw std::runtime_error( "a 'shape' item is required" );

      }
      _shape = ( float )p->second->getScalar();

      p = parameters.find( "mean" );
      if ( p != parameters.end() )
      {

        throw std::runtime_error(
                   "'mean' item incompatible with 'shape' and 'scale' items" );

      }

      p = parameters.find( "stddev" );
      if ( p != parameters.end() )
      {

        throw std::runtime_error(
                   "'stddev' item incompatible with 'shape' and 'scale' items" );

      }

    }

    p = parameters.find( "multiplicator" );
    if ( p != parameters.end() )
    {

      _multiplicator = ( float )p->second->getScalar();

    }
    else
    {

      _multiplicator = 1.0f;

    }

    _scaleTimesMultiplicator = _scale * _multiplicator;

    p = parameters.find( "upper_boundary" );
    if ( p != parameters.end() )
    {

      _hasUpperBoundary = true;
      _upperBoundary = ( float )p->second->getScalar();

    }
    else
    {

      _hasUpperBoundary = false;
      _upperBoundary = 1e38f;

    }

  }
  GKG_CATCH( "gkg::GammaDistribution::GammaDistribution( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GammaDistribution::~GammaDistribution()
{
}



float gkg::GammaDistribution::getRandomValue() const
{

  try
  {

    float value = ( float )_factory->getGammaRandomNumber(
                                    _randomGenerator,
                                    _shape,
                                    _scaleTimesMultiplicator ) / _multiplicator;
    if ( _hasUpperBoundary )
    {

      while ( value > _upperBoundary )
      {

        value = ( float )_factory->getGammaRandomNumber(
                                    _randomGenerator,
                                    _shape,
                                    _scaleTimesMultiplicator ) / _multiplicator;

      }

    }

    return value;


  }
  GKG_CATCH( "float gkg::GammaDistribution::getRandomValue() const" );
}


std::string gkg::GammaDistribution::getStaticName()
{

  try
  {

    return "gamma-distribution";

  }
  GKG_CATCH( "std::string gkg::GammaDistribution::getStaticName()" );

}


//
// adding the GammaDistribution creator to the distribution factory
//

RegisterDistributionCreator( gkg, GammaDistribution );
