#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingMCMCParameterSet.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEDefines.h>
#include <gkg-core-exception/Exception.h>


gkg::CTOGSEAxonMappingMCMCParameterSet::CTOGSEAxonMappingMCMCParameterSet( 
                                               const gkg::Vector& lowerBounds,
                                               const gkg::Vector& upperBounds,
                                               const std::vector< bool >& fixed,
                                               const gkg::Vector& steps )
                                       : AxonMappingMCMCParameterSet(
                                                                    lowerBounds,
                                                                    upperBounds,
                                                                    fixed,
                                                                    steps )
{
}


gkg::CTOGSEAxonMappingMCMCParameterSet::~CTOGSEAxonMappingMCMCParameterSet()
{
}


bool gkg::CTOGSEAxonMappingMCMCParameterSet::updateParameters()
{

  try
  {

    int32_t p = 0;
    for ( p = CTOGSE_IsotropicFraction; p < CTOGSE_PrincipalOrientation; p++ )
    {

      if ( this->_fixed[ p ] == false )
      { 

        this->_newParameters( p ) = this->_currentParameters( p ) +
                                    this->_steps( p ) *
                                    this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
        while ( ( this->_newParameters( p ) < this->_lowerBounds( p ) ) ||
                ( this->_newParameters( p ) > this->_upperBounds( p ) ) )
        {

          this->_newParameters( p ) = this->_currentParameters( p ) +
                                      this->_steps( p ) *
                                      this->_factory->getGaussianRandomNumber(
                                                        *this->_randomGenerator,
                                                        0.0,
                                                        1.0 );
          //return false;

        }

      }
      else
      {

        this->_newParameters( p ) = this->_currentParameters( p );

      }

    }

    //  specific case of principal orientation
    if ( this->_fixed[ CTOGSE_PrincipalOrientation ] == false )
    {

      this->_newParameters( CTOGSE_PrincipalOrientation ) =
                   this->_currentParameters( CTOGSE_PrincipalOrientation ) +
                   this->_steps( CTOGSE_PrincipalOrientation ) *
                   this->_factory->getGaussianRandomNumber(
                                                    *this->_randomGenerator,
                                                    0.0,
                                                    1.0 );
      this->_newParameters( CTOGSE_PrincipalOrientation + 1 ) =
                  this->_currentParameters( CTOGSE_PrincipalOrientation + 1 ) +
                  this->_steps( CTOGSE_PrincipalOrientation + 1 ) *
                  this->_factory->getGaussianRandomNumber(
                                                   *this->_randomGenerator,
                                                   0.0,
                                                   1.0 );
      this->_newParameters( CTOGSE_PrincipalOrientation + 2 ) =
                  this->_currentParameters( CTOGSE_PrincipalOrientation + 2 ) +
                  this->_steps( CTOGSE_PrincipalOrientation + 2 ) *
                  this->_factory->getGaussianRandomNumber(
                                                   *this->_randomGenerator,
                                                   0.0,
                                                   1.0 );

      double norm = std::sqrt(
              this->_newParameters( CTOGSE_PrincipalOrientation ) *
              this->_newParameters( CTOGSE_PrincipalOrientation ) +
              this->_newParameters( CTOGSE_PrincipalOrientation + 1 ) *
              this->_newParameters( CTOGSE_PrincipalOrientation + 1 ) +
              this->_newParameters( CTOGSE_PrincipalOrientation + 2 ) *
              this->_newParameters( CTOGSE_PrincipalOrientation + 2 ) );
      this->_newParameters( CTOGSE_PrincipalOrientation ) /= norm;
      this->_newParameters( CTOGSE_PrincipalOrientation + 1 ) /= norm;
      this->_newParameters( CTOGSE_PrincipalOrientation + 2 ) /= norm;

    }
    else
    {

      this->_newParameters( CTOGSE_PrincipalOrientation ) =
                  this->_currentParameters( CTOGSE_PrincipalOrientation );
      this->_newParameters( CTOGSE_PrincipalOrientation + 1 ) =
                  this->_currentParameters( CTOGSE_PrincipalOrientation + 1 );
      this->_newParameters( CTOGSE_PrincipalOrientation + 2 ) =
                  this->_currentParameters( CTOGSE_PrincipalOrientation + 2 );

    }

    return true;

  }
  GKG_CATCH( "bool "
             "gkg::CTOGSEAxonMappingMCMCParameterSet::"
             "updateParameters()" );

}
