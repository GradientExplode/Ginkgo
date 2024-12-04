#include <gkg-qmri-t2/EKFFunctionT2.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::EKFFunctionT2::EKFFunctionT2()
                  : KFFunction( 2 )
{
}


void gkg::EKFFunctionT2::getValueAt( const gkg::Vector& X, double& Y ) const
{

  try
  {

    Y = 0.0;

    if ( std::fabs( X( 1 ) ) >= 1e-4 )
    {

      Y = X( 0 ) * std::exp( -_parameters( 1 ) / X( 1 ) );

    }

  }
  GKG_CATCH( "void gkg::EKFFunctionT2::getValueAt( "
             "const gkg::Vector& X, double& Y ) const" );

}


void gkg::EKFFunctionT2::getJacobian( const gkg::Vector& X,
                                      gkg::Vector& J ) const
{

  try 
  {

    float t = _parameters( 1 );

    if ( std::fabs( X( 1 ) ) < 1e-4 )
    {

      J( 0 ) = 0.0;
      J( 1 ) = 0.0;

    }
    else
    {

      float exp1 = std::exp( -t / X( 1 ) );
    
      J( 0 ) =  exp1;
      J( 1 ) =  X( 0 ) * t / X( 1 ) / X( 1 ) * exp1;

    }

  }
  GKG_CATCH( "void gkg::EKFFunctionT2::getJacobian( "
             "const gkg::Vector& X, gkg::Vector& J )" );

}
