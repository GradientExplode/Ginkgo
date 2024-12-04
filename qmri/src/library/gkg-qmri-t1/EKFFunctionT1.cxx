#include <gkg-qmri-t1/EKFFunctionT1.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::EKFFunctionT1::EKFFunctionT1()
                  : KFFunction( 2 )
{
}


void gkg::EKFFunctionT1::getValueAt( const gkg::Vector& X, double& Y ) const
{

  try
  {

    if ( std::fabs( X( 1 ) ) >= 1e-4 )
    {

      Y = std::fabs( X( 0 ) * 
                     ( 1.0 - 2.0 * std::exp( -_parameters( 1 ) / X( 1 ) ) ) );

    }
    else
    {

      Y = X( 0 );

    }

  }
  GKG_CATCH( "void gkg::EKFFunctionT1::getValueAt( "
             "const gkg::Vector& X, double& Y ) const" );

}


void gkg::EKFFunctionT1::getJacobian( const gkg::Vector& X, 
                                      gkg::Vector& J ) const

{

  try 
  {

    float t = _parameters( 1 );
    float fact = ( t <= ( X( 1 ) * std::log( 2 ) ) ) ? -1.0 : 1.0;

    if ( std::fabs( X( 1 ) ) >= 1e-4 )
    {

      float exp1 = 2.0 * std::exp( -t / X( 1 ) );
    
      J( 0 ) =  fact * ( 1.0 - exp1 );
      J( 1 ) =  -fact * X( 0 ) * t / X( 1 ) / X( 1 ) * exp1;
  
    }
    else
    {

      J( 0 ) = fact;
      J( 1 ) = 0.0;

    }

  }
  GKG_CATCH( "void gkg::EKFFunctionT1::getJacobian( "
             "const gkg::Vector& X, gkg::Vector& J )" );

}
