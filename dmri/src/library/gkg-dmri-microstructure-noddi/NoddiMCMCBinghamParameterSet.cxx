#include <gkg-dmri-microstructure-noddi/NoddiMCMCBinghamParameterSet.h>
#include <gkg-core-exception/Exception.h>


gkg::NoddiMCMCBinghamParameterSet::NoddiMCMCBinghamParameterSet( 
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


gkg::NoddiMCMCBinghamParameterSet::~NoddiMCMCBinghamParameterSet()
{
}


bool gkg::NoddiMCMCBinghamParameterSet::updateParameters()
{

  try
  {

    int32_t p = 0;
    for ( p = 0; p < 4; p++ )
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
    if ( this->_fixed[ 4 ] == false )
    {

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
      this->_newParameters( 6 ) = this->_currentParameters( 6 ) +
                                    this->_steps( 6 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );

      double norm = std::sqrt( this->_newParameters( 4 ) *
                               this->_newParameters( 4 ) +
                               this->_newParameters( 5 ) *
                               this->_newParameters( 5 ) +
                               this->_newParameters( 6 ) *
                               this->_newParameters( 6 ) );
      this->_newParameters( 4 ) /= norm;
      this->_newParameters( 5 ) /= norm;
      this->_newParameters( 6 ) /= norm;

    }
    else
    {

      this->_newParameters( 4 ) = this->_currentParameters( 4 );
      this->_newParameters( 5 ) = this->_currentParameters( 5 );
      this->_newParameters( 6 ) = this->_currentParameters( 6 );

    }

    //  specific case of secondary orientation
    if ( this->_fixed[ 7 ] == false )
    {

      this->_newParameters( 7 ) = this->_currentParameters( 7 ) +
                                    this->_steps( 7 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( 8 ) = this->_currentParameters( 8 ) +
                                    this->_steps( 8 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( 9 ) = this->_currentParameters( 9 ) +
                                    this->_steps( 9 ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );

      double norm = std::sqrt( this->_newParameters( 7 ) *
                               this->_newParameters( 7 ) +
                               this->_newParameters( 8 ) *
                               this->_newParameters( 8 ) +
                               this->_newParameters( 9 ) *
                               this->_newParameters( 9 ) );
      this->_newParameters( 7 ) /= norm;
      this->_newParameters( 8 ) /= norm;
      this->_newParameters( 9 ) /= norm;

    }
    else
    {

      this->_newParameters( 7 ) = this->_currentParameters( 7 );
      this->_newParameters( 8 ) = this->_currentParameters( 8 );
      this->_newParameters( 9 ) = this->_currentParameters( 9 );

    }

    for ( p = 10; p < this->_parameterCount; p++ )
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
             "gkg::NoddiMCMCBinghamParameterSet::"
             "updateParameters()" );

}
