#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/ExtendedKalmanFilter.h>
#include <gkg-processing-numericalanalysis/UnscentedKalmanFilter.h>
#include <gkg-qmri-t2/EKFFunctionT2.h>
#include <gkg-qmri-t2/UKFFunctionT2.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>


int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    std::string type = "KF";
    int32_t signal = 1;
    double kappa = 0.01;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, 
                                  "Test Relaxometry T2 Kalman's filter" );
    application.addSingleOption( "-type",
                                 "Kalman type KF / EKF / UKF "
                                 "(default = KF)",
                                 type,
                                 true );
    application.addSingleOption( "-s",
                                 "test signal 1 / 2 / 3 "
                                 "(default = 1)",
                                 signal,
                                 true );
    application.addSingleOption( "-k",
                                 "kappa (UKF only) "
                                 "(default = 0.01)",
                                 kappa,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    // These data come from T2 relaxometry ( S = S0 * exp( -t / T2 ) )
    int16_t sG1[] = { 1335, 934, 711, 563, 373, 302, 294, 165, 158, 58 };
    int16_t sG2[] = 
                 { 2260, 1940, 1836, 1746, 1557, 1570, 1366, 1270, 1230, 1231 };
    int16_t sB1[] = { 1135, 901, 649, 469, 424, 318, 281, 172, 168, 109 };
    int16_t* s = sG1;
    int32_t r, rankCount = 10;
    double minT = 30.0;
    double maxT = 200.0;
    double deltaT = ( maxT - minT ) / double( rankCount - 1 );
    gkg::Kalman::Parameters parameters( 2 );
    gkg::Kalman* kalmanFilter = 0;
    double t = minT;

    switch ( signal )
    {

      case 3:
        s = sB1;
        break;

      case 2:
        s = sG2;
        break;

      case 1:
      default:
        s = sG1;
        break;

    }

    if ( type == "EKF" )
    {

      kalmanFilter = new gkg::ExtendedKalmanFilter(
                gkg::RCPointer< gkg::KFFunction >( new gkg::EKFFunctionT2() ) );
      parameters.X( 0 ) = *s;
      parameters.X( 1 ) = 100.0;
      parameters.varianceOfX( 0, 0 ) = 1e7;
      parameters.varianceOfX( 1, 1 ) = 5e3;

    }
    else if ( type == "UKF" )
    {

      if ( kappa < 0.0 )
      {

        kappa = 0.01;

      }

      kalmanFilter = new gkg::UnscentedKalmanFilter(
                  gkg::RCPointer< gkg::KFFunction >( new gkg::UKFFunctionT2() ),
                  kappa );
      parameters.X( 0 ) = *s;
      parameters.X( 1 ) = 100.0;
      parameters.varianceOfX( 0, 0 ) = 1e7;
      parameters.varianceOfX( 1, 1 ) = 1e3;

    }
    else
    {

      kalmanFilter = new gkg::Kalman(
                gkg::RCPointer< gkg::KFFunction >( new gkg::KFFunction( 2 ) ) );

    }

    std::cout << std::setw( 6 ) << "Signal"
              << std::setw( 12 ) << "t"
              << std::setw( 12 ) << "y"
              << std::setw( 12 ) << "X0"
              << std::setw( 12 ) << "X1"
              << std::setw( 12 ) << "S0"
              << std::setw( 12 ) << "T" << std::endl;
    std::cout << std::setw( 6 ) << s[ 0 ]
              << std::setw( 12 ) << t
              << std::setw( 12 ) << ( ( type == "KF" ) ? 
                                      std::log( double( s[ 0 ] ) ) :
                                      double( s[ 0 ] ) )
              << std::setw( 12 ) << parameters.X( 0 )
              << std::setw( 12 ) << parameters.X( 1 )
              << std::setw( 12 ) << "---"
              << std::setw( 12 ) << "---"
              << std::endl;

    for ( r = 0; r < rankCount; r++, t += deltaT )
    {

      gkg::Vector R( 2 );

      R( 0 ) = 1.0;
      R( 1 ) = t;

      kalmanFilter->setSubDesignMatrix( R );

      double y = ( type == "KF" ) ? std::log( double( s[ r ] ) )
                                  : double( s[ r ] );

      kalmanFilter->iterate( parameters, y, r );

      double S0, T;

      if ( type == "KF" )
      {

        S0 = std::max( std::min( std::exp( parameters.X( 0 ) ), 4095.0 ), 0.0 );
        T = std::max( std::min( -1.0 / parameters.X( 1 ), 500.0 ), 0.0 );

      }
      else
      {

        S0 = std::max( std::min( parameters.X( 0 ), 4095.0 ), 0.0 );
        T = std::max( std::min( parameters.X( 1 ), 500.0 ), 0.0 );

      }

      std::cout << std::setw( 6 ) << s[ r ]
                << std::setw( 12 ) << t
                << std::setw( 12 ) << y
                << std::setw( 12 ) << parameters.X( 0 )
                << std::setw( 12 ) << parameters.X( 1 )
                << std::setw( 12 ) << S0
                << std::setw( 12 ) << T
                << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
