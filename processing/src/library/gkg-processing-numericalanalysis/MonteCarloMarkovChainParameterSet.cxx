#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainParameterSet.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::MonteCarloMarkovChainParameterSet::MonteCarloMarkovChainParameterSet(
                                               int32_t parameterCount,
                                               const gkg::Vector& lowerBounds,
                                               const gkg::Vector& upperBounds,
                                               const std::vector< bool >& fixed,
                                               const gkg::Vector& steps )
                                       : _parameterCount( parameterCount ),
                                         _currentParameters( parameterCount ),
                                         _newParameters( parameterCount ),
                                         _lowerBounds( lowerBounds ),
                                         _upperBounds( upperBounds ),
                                         _fixed( fixed ),
                                         _steps( steps )
{

  try
  {

    _currentParameters.fill( 0.0 );
    _newParameters.fill( 0.0 );

    //sanity check(s)
    int32_t p = 0;
    for ( p = 0; p < _parameterCount; p++ )
    {

      if ( _lowerBounds( p ) >= _upperBounds( p ) )
      {

        throw std::runtime_error(
                                    std::string( "lower bound of parameter " ) +
                                    gkg::StringConverter::toString( p ) +
                                    " is greater or equal to its upper bound" );

      }

      if ( _steps( p ) <= 0 )
      {

        throw std::runtime_error( std::string( "step of parameter " ) +
                                  gkg::StringConverter::toString( p ) +
                                  " must be strictly positive" );

      }

    }



  }
  GKG_CATCH( "gkg::MonteCarloMarkovChainParameterSet::"
             "MonteCarloMarkovChainParameterSet( "
             "int32_t parameterCount, "
             "const gkg::Vector& lowerBounds, "
             "const gkg::Vector& upperBounds, "
             "const std::vector< bool >& fixed, "
             "const gkg::Vector& steps )" );

}


gkg::MonteCarloMarkovChainParameterSet::~MonteCarloMarkovChainParameterSet()
{
}


int32_t gkg::MonteCarloMarkovChainParameterSet::getParameterCount() const
{

  try
  {

    return _parameterCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::MonteCarloMarkovChainParameterSet::getParameterCount() "
             "const" );

}


const gkg::Vector& 
gkg::MonteCarloMarkovChainParameterSet::getCurrentParameters() const
{

  try
  {

    return _currentParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::MonteCarloMarkovChainParameterSet::getCurrentParameters() "
             "const" );

}


const gkg::Vector& 
gkg::MonteCarloMarkovChainParameterSet::getNewParameters() const
{

  try
  {

    return _newParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::MonteCarloMarkovChainParameterSet::getNewParameters() "
             "const" );

}


const gkg::Vector& 
gkg::MonteCarloMarkovChainParameterSet::getLowerBounds() const
{

  try
  {

    return _lowerBounds;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::MonteCarloMarkovChainParameterSet::getLowerBounds() const" );

}


const gkg::Vector& 
gkg::MonteCarloMarkovChainParameterSet::getUpperBounds() const
{

  try
  {

    return _upperBounds;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::MonteCarloMarkovChainParameterSet::getUpperBounds() const" );

}



const std::vector< bool >& 
gkg::MonteCarloMarkovChainParameterSet::getFixed() const
{

  try
  {

    return _fixed;

  }
  GKG_CATCH( "const std::vector< bool >& "
             "gkg::MonteCarloMarkovChainParameterSet::getFixed() const" );

}


const gkg::Vector& gkg::MonteCarloMarkovChainParameterSet::getSteps() const
{

  try
  {

    return _steps;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::MonteCarloMarkovChainParameterSet::getSteps() const" );

}


void gkg::MonteCarloMarkovChainParameterSet::setCurrentParameters(
                                                     const gkg::Vector& others )
{

  try
  {

    if ( others.getSize() != _parameterCount )
    {

      throw std::runtime_error( "inconsistent size(s)" );

    }

    _currentParameters = others;

  }
  GKG_CATCH( "void gkg::MonteCarloMarkovChainParameterSet::"
             "setCurrentParameters( const gkg::Vector& others )" );

}


void gkg::MonteCarloMarkovChainParameterSet::acceptNewParameters()
{

  try
  {

    _currentParameters = _newParameters;

  }
  GKG_CATCH( "void "
             "gkg::MonteCarloMarkovChainParameterSet::acceptNewParameters()" );



}



