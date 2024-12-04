#include <gkg-dmri-sequence-tuning/PGSESequenceTuning.h>


#include <gkg-core-exception/Exception.h>
#include <cmath>


#define MT_PER_M_TO_G_PER_CM              0.1
#define T_PER_M_PER_S_TO_G_PER_CM_PER_US  0.0001
#define MS_TO_US                          1000.0

#define TWOPI_GAM 26748.0
#define GAM 26748.0 / ( 2 * M_PI )

#define RUP_GRD( A, GRAD_UPDATE_TIME )                                         \
                                 ( ( GRAD_UPDATE_TIME - ( ( int32_t )( A ) %   \
                                     GRAD_UPDATE_TIME ) ) % GRAD_UPDATE_TIME + \
                                   ( int32_t )( A ) )
/*
//
// Stejskal-Tanner' equation for trapezoidal gradients:
//
//  targetAmp ----> +--------------+            +--------------+
//                 /                \          /                \
//                /                  \        /                  \
//            ---+--+--------------+--+------+--+--------------+--+--------
//
//  rampWidth ---<->-
//                    pulseWidth      timeSeparation
//               <--------------------><----->
//
*/

void gkg::getPGSEParameters( double maximumGradientAmplitude,      // in mT/m
                             double minimumSlewRate,               // in T/m/s
                             double gradientResolution,            // in us
                             double timeSeparation,                // in ms
                             double timeBeforeEcho,                // in ms
                             double bValue,                        // in s/mm2
                             double& littleDelta,                  // in ms
                             double& bigDelta,                     // in ms
                             double& rampWidth,                    // in ms
                             double& diffusionTime,                // in ms
                             double& echoTime )                    // in ms
{

  try
  {

    maximumGradientAmplitude *= MT_PER_M_TO_G_PER_CM;
    minimumSlewRate *= T_PER_M_PER_S_TO_G_PER_CM_PER_US;
    timeSeparation *= MS_TO_US;

    rampWidth = ( double )RUP_GRD( ( int32_t )( maximumGradientAmplitude /
                                                minimumSlewRate ),
                                   ( int32_t )gradientResolution );

    // solving the equation b = (GAM)^2*g^2*(d^2*(D - d/3.0)+r^3/30 - d*r^2/6)
    // for "d" which results in a cubic equation (the r^ are the ramp times to
    // account for the trapezoid diffusion lobes):
    //
    // d^3 - 3D*d^2 + dr^2/2B -(r^3/(10*B) -3b/beta) = 0 
    // where beta = (GAM*g)^2 and D = (pw_sep+d).  Thus, the equ. becomes..
    //
    // d^3 + (3/2*pw_sep)*d^2 - (r^2/4)*d - (3*b/2beta - r^3/20) = 0

    // casting 180/crusher time in seconds
    double sep = ( double )( timeSeparation / 1.0e6 );  

    // diffusion ramps in seconds
    double ramp = ( double )( rampWidth / 1.0e6 );

    double beta = TWOPI_GAM * TWOPI_GAM * ( maximumGradientAmplitude / 10.0 ) *
                  ( maximumGradientAmplitude / 10.0 );
    double a = ( 3.0 / 2.0 ) * sep;
    double b = ramp * ramp / 4;
    double c = ( ( -3.0 / 2.0 ) * bValue ) / beta +
               ( ( ramp * ramp * ramp ) / 20.0 );

    double Q = ( a * a - 3.0 * b ) / 9.0;
    double R = ( ( 2.0 * ( a * a * a ) - 9.0 * a * b + 27.0 * c ) / 54.0 );
    double Rsq = R * R;  
    double Qcub = Q * Q * Q;

    double theta = 0.0;
    double root[ 3 ];
    double pulseWidth = 0.0;

    if ( Rsq < Qcub )
    {

      theta = std::acos( R / std::sqrt( Qcub ) );

      // casting roots in us
      root[ 0 ] = 1e6 * ( -2.0 * std::sqrt( Q ) *
                  std::cos( theta / 3.0 ) - a / 3.0 );
      root[ 1 ] = 1e6 * ( -2.0 * std::sqrt( Q ) *
                  std::cos( ( theta + 2.0 * M_PI ) / 3.0 ) - a / 3.0 );
      root[ 2 ] = 1e6 * ( -2.0 * std::sqrt( Q ) *
                  std::cos( ( theta - 2.0 * M_PI ) / 3.0 ) - a / 3.0 );
  
      // order roots and take the smallest - non negative value
      int32_t i = 0;
      int32_t j = 0;
      double temp = 0.0;
      for ( i = 0; i <= 2; i++ )
      {

        temp = root[ i ];
        for ( j = i + 1; j <= 2; j++ )
        {

          if ( root[ j ] < temp )
          {

            root[ i ] = root[ j ];
            root[ j ] = temp;
            temp = root[ i ];

          }

        }

      }
      
      // take smallest that is larger than gradientResolution
      if ( root[ 0 ] >= gradientResolution )
      {

        pulseWidth = ( double )( ( int32_t )root[ 0 ] );

      }
      else if ( root[ 1 ] >= gradientResolution )
      {

        pulseWidth = ( double )( ( int32_t )root[ 1 ] );

      }
      else if ( root[ 2 ] >= gradientResolution )
      {

        pulseWidth = ( double )( ( int32_t )root[ 2 ] );

      }
      else
      {

        throw std::runtime_error(
                        "pulse width would be lower than gradient resolution" );

      }

    }
    else
    {

      double sign = -1.0;
      // checking sign of R
      if ( R >= 0 )
      {

        sign = 1.0;

      }

      double A = -sign * std::cbrt( std::fabs( R ) + std::sqrt( Rsq - Qcub ) );
      double B = Q / A;

      // this is the only real root - the other two are complex
      root[ 0 ] = 1e6 * ( A + B - a / 3.0 );
 
      if ( root[ 0 ] >= gradientResolution )
      {

        pulseWidth = ( double )( ( int32_t )root[ 0 ] );

      }
      else
      {

        throw std::runtime_error(
                        "pulse width would be lower than gradient resolution" );

      }

    }

    // put pulseWidth on gradient resolution boundaries
    pulseWidth = ( double )RUP_GRD( pulseWidth, ( int32_t )gradientResolution );

    littleDelta = ( pulseWidth + 2 * rampWidth ) / MS_TO_US;
    bigDelta = ( pulseWidth + 2 * rampWidth + timeSeparation ) / MS_TO_US;
    diffusionTime = ( bigDelta - littleDelta / 3.0 );
    echoTime = 2.0 * ( 0.5 * timeSeparation / MS_TO_US +
                       pulseWidth / MS_TO_US + timeBeforeEcho );

  }
  GKG_CATCH( "void gkg::getPGSEParameters( "
             "double maximumGradientAmplitude, "
             "double minimumSlewRate, "
             "double gradientResolution, "
             "double timeSeparation, "
             "double timeBeforeEcho, "
             "double bValue, "
             "double& littleDelta, "
             "double& bigDelta, "
             "double& rampWidth, "
             "double& diffusionTime, "
             "double& echoTime )" );

}


#undef MT_PER_M_TO_G_PER_CM
#undef T_PER_M_PER_S_TO_G_PER_CM_PER_US
#undef MS_TO_US


#undef TWOPI_GAM
#undef GAM

#undef RUP_GRD
