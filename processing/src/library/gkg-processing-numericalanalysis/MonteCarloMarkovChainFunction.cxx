#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::MonteCarloMarkovChainFunction::MonteCarloMarkovChainFunction()
{
}


gkg::MonteCarloMarkovChainFunction::~MonteCarloMarkovChainFunction()
{
}


void gkg::MonteCarloMarkovChainFunction::getValuesAt(
                                         const std::vector< float >& parameters,
                                         std::vector< float >& values ) const
{

  try
  {

    if ( values.empty() )
    {

      throw std::runtime_error( "empty vector to return values" );

    }

    int32_t size = ( int32_t )values.size();
    gkg::Vector vectorOfValues( size );
    this->getValuesAt( gkg::Vector( parameters ), vectorOfValues );

    int32_t i = 0;
    for ( i = 0; i < size; i++ )
    {

      values[ i ] = vectorOfValues( i );

    }

  }
  GKG_CATCH( "void gkg::MonteCarloMarkovChainFunction::getValuesAt( "
             "const std::vector< float >& parameters, "
             "std::vector< float >& values ) const" );

}


void gkg::MonteCarloMarkovChainFunction::getValuesAt(
                                        const std::vector< double >& parameters,
                                        std::vector< double >& values ) const
{

  try
  {

    if ( values.empty() )
    {

      throw std::runtime_error( "empty vector to return values" );

    }

    int32_t size = ( int32_t )values.size();
    gkg::Vector vectorOfValues( size );
    this->getValuesAt( gkg::Vector( parameters ), vectorOfValues );

    int32_t i = 0;
    for ( i = 0; i < size; i++ )
    {

      values[ i ] = vectorOfValues( i );

    }

  }
  GKG_CATCH( "void gkg::MonteCarloMarkovChainFunction::getValuesAt( "
             "const std::vector< double >& parameters, "
             "std::vector< double >& values ) const" );

}
