#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonParameterSet.h>
#include <gkg-core-exception/Exception.h>


gkg::NoddiMCMCWatsonParameterSet::NoddiMCMCWatsonParameterSet( 
                                               const gkg::Vector& lowerBounds,
                                               const gkg::Vector& upperBounds,
                                               const std::vector< bool >& fixed,
                                               const gkg::Vector& steps )
                                 : NoddiMCMCParameterSet( lowerBounds,
                                                          upperBounds,
                                                          fixed,
                                                          steps )
{
}


gkg::NoddiMCMCWatsonParameterSet::~NoddiMCMCWatsonParameterSet()
{
}


bool gkg::NoddiMCMCWatsonParameterSet::updateParameters()
{

  try
  {

    int32_t p = 0;
    for ( p = 0; p < 3; p++ )
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

    //  specific case of principal orientation
    if ( this->_fixed[ 3 ] == false )
    {

      this->_newParameters( 3 ) = this->_currentParameters( 3 ) +
                                    this->_steps( 3 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( 4 ) = this->_currentParameters( 4 ) +
                                    this->_steps( 4 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( 5 ) = this->_currentParameters( 5 ) +
                                    this->_steps( 5 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );

      double norm = std::sqrt( this->_newParameters( 3 ) *
                               this->_newParameters( 3 ) +
                               this->_newParameters( 4 ) *
                               this->_newParameters( 4 ) +
                               this->_newParameters( 5 ) *
                               this->_newParameters( 5 ) );
      this->_newParameters( 3 ) /= norm;
      this->_newParameters( 4 ) /= norm;
      this->_newParameters( 5 ) /= norm;

    }
    else
    {

      this->_newParameters( 3 ) = this->_currentParameters( 3 );
      this->_newParameters( 4 ) = this->_currentParameters( 4 );
      this->_newParameters( 5 ) = this->_currentParameters( 5 );

    }

    for ( p = 6; p < this->_parameterCount; p++ )
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
  GKG_CATCH( "bool "
             "gkg::NoddiMCMCWatsonParameterSet::"
             "updateParameters()" );

}
