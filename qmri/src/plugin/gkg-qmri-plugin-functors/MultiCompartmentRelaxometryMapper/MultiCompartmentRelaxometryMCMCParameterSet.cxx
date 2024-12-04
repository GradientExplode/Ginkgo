#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCParameterSet.h>
#include <gkg-core-exception/Exception.h> 


gkg::MultiCompartmentRelaxometryMCMCParameterSet::MultiCompartmentRelaxometryMCMCParameterSet(
                                               const gkg::Vector& lowerBounds, 
                                               const gkg::Vector& upperBounds, 
                                               const std::vector< bool >& fixed, 
                                               const gkg::Vector& steps ) 
                                       : gkg::MonteCarloMarkovChainParameterSet( 
                                                          lowerBounds.getSize(), 
                                                          lowerBounds, 
                                                          upperBounds, 
                                                          fixed, 
                                                          steps ) 
{

  try
  {
  
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset( 
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) ); 

  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryMCMCParameterSet::"
             "MultiCompartmentRelaxometryMCMCParameterSet( "
             "const gkg::Vector& lowerBounds, " 
             "const gkg::Vector& upperBounds, "
             "const std::vector< bool >& fixed, "
             "const gkg::Vector& steps )" ); 

}


gkg::MultiCompartmentRelaxometryMCMCParameterSet::~MultiCompartmentRelaxometryMCMCParameterSet()
{
}


bool gkg::MultiCompartmentRelaxometryMCMCParameterSet::updateParameters()
{
  
  try
  {
    
    int32_t p = 0; 
    for ( p = 0; p < this->_parameterCount; p++ )
    {
      
      if ( this->_fixed[ p ] == false )
      {
        
        this->_newParameters( p ) = this->_currentParameters( p ) + 
                                    this->_steps( p ) * 
                                    this->_factory->getGaussianRandomNumber( 
                                                        *this->_randomGenerator, 
                                                        0.0, 
                                                        1.0 );
        if ( ( this->_newParameters( p ) < this->_lowerBounds( p ) ) ||
             ( this->_newParameters( p ) > this->_upperBounds( p ) ) )
        {

          return false;

        }

      }
      else
      {

        this->_newParameters( p ) = this->_currentParameters( p );

      }

    }

    return true;

  }
  GKG_CATCH( "bool gkg::MultiCompartmentRelaxometryMCMCParameterSet::"
             "updateParameters()" );

}