#include <gkg-qmri-t2/UKFFunctionT2.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UKFFunctionT2::UKFFunctionT2()
                  : KFFunction( 2 )
{
}


void gkg::UKFFunctionT2::getValuesAt( const gkg::Matrix& X, 
                                      gkg::Vector& Y ) const

{

  try 
  {

    float t = _parameters( 1 );
    int32_t i, sigmaPointCount = X.getSize2();

    Y.setZero();

    for ( i = 0; i < sigmaPointCount; i++ )
    {

      if ( std::fabs( X( 1, i ) ) >= 1e-4 )
      {

        Y( i ) = X( 0, i ) * std::exp( -t / X( 1, i ) );

      }

    }
  
  }
  GKG_CATCH( "void gkg::UKFFunctionT2::getValuesAt( "
             "const gkg::Matrix& X, gkg::Vector& Y )" );

}
