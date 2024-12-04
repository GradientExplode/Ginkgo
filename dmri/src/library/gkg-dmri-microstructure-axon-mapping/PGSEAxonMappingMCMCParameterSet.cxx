#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingMCMCParameterSet.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEDefines.h>
#include <gkg-core-exception/Exception.h>


gkg::PGSEAxonMappingMCMCParameterSet::PGSEAxonMappingMCMCParameterSet( 
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


gkg::PGSEAxonMappingMCMCParameterSet::~PGSEAxonMappingMCMCParameterSet()
{
}


bool gkg::PGSEAxonMappingMCMCParameterSet::updateParameters()
{

  try
  {

    int32_t p = 0;
    for ( p = PGSE_IsotropicFraction; p < PGSE_PrincipalOrientation; p++ )
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
    if ( this->_fixed[ PGSE_PrincipalOrientation ] == false )
    {

      this->_newParameters( PGSE_PrincipalOrientation ) =
                     this->_currentParameters( PGSE_PrincipalOrientation ) +
                     this->_steps( PGSE_PrincipalOrientation ) *
                     this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( PGSE_PrincipalOrientation + 1 ) =
                     this->_currentParameters( PGSE_PrincipalOrientation + 1 ) +
                     this->_steps( PGSE_PrincipalOrientation + 1 ) *
                     this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );
      this->_newParameters( PGSE_PrincipalOrientation + 2 ) =
                     this->_currentParameters( PGSE_PrincipalOrientation + 2 ) +
                     this->_steps( PGSE_PrincipalOrientation + 2 ) *
                     this->_factory->getGaussianRandomNumber(
                                                      *this->_randomGenerator,
                                                      0.0,
                                                      1.0 );

      double norm = std::sqrt(
              this->_newParameters( PGSE_PrincipalOrientation ) *
              this->_newParameters( PGSE_PrincipalOrientation ) +
              this->_newParameters( PGSE_PrincipalOrientation + 1 ) *
              this->_newParameters( PGSE_PrincipalOrientation + 1 ) +
              this->_newParameters( PGSE_PrincipalOrientation + 2 ) *
              this->_newParameters( PGSE_PrincipalOrientation + 2 ) );
      this->_newParameters( PGSE_PrincipalOrientation ) /= norm;
      this->_newParameters( PGSE_PrincipalOrientation + 1 ) /= norm;
      this->_newParameters( PGSE_PrincipalOrientation + 2 ) /= norm;

    }
    else
    {

      this->_newParameters( PGSE_PrincipalOrientation ) =
                  this->_currentParameters( PGSE_PrincipalOrientation );
      this->_newParameters( PGSE_PrincipalOrientation + 1 ) =
                  this->_currentParameters( PGSE_PrincipalOrientation + 1 );
      this->_newParameters( PGSE_PrincipalOrientation + 2 ) =
                  this->_currentParameters( PGSE_PrincipalOrientation + 2 );

    }

    return true;

  }
  GKG_CATCH( "bool "
             "gkg::PGSEAxonMappingMCMCParameterSet::"
             "updateParameters()" );

}
