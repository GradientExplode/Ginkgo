#include <gkg-qmri-t1/UKFFunctionT1.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UKFFunctionT1::UKFFunctionT1()
                  : KFFunction( 2 )
{
}


void gkg::UKFFunctionT1::getValuesAt( const gkg::Matrix& X, 
                                      gkg::Vector& Y ) const

{

  try 
  {

    float t = _parameters( 1 );
    int32_t i, sigmaPointCount = X.getSize2();

    for ( i = 0; i < sigmaPointCount; i++ )
    {

      if ( std::fabs( X( 1, i ) ) >= 1e-4 )
      {

        Y( i ) = std::fabs( X( 0, i ) * 
                            ( 1.0 - 2.0 * std::exp( -t / X( 1, i ) ) ) );

      }
      else
      {

        Y( i ) = X( 0, i );

      }

    }
  
  }
  GKG_CATCH( "void gkg::UKFFunctionT1::getValuesAt( "
             "const gkg::Matrix& X, gkg::Vector& Y )" );

}
