#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/ExtendedKalmanFilter.h>
#include <gkg-processing-numericalanalysis/UnscentedKalmanFilter.h>
#include <gkg-qmri-t1/EKFFunctionT1.h>
#include <gkg-qmri-t1/UKFFunctionT1.h>
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

    std::string type = "EKF";
    int32_t signal = 1;
    double kappa = 0.01;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, 
                                  "Test Relaxometry T1 Kalman's filter" );
    application.addSingleOption( "-type",
                                 "Kalman type EKF / UKF "
                                 "(default = EKF)",
                                 type,
                                 true );
    application.addSingleOption( "-s",
                                 "test signal 1 / 2 "
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

    // These data come from T1 relaxometry 
    // ( S = | S0 * ( 1 - 2 * exp( -t / T1 ) ) | )
    int16_t sG1[] = { 668, 180, 165, 546, 713, 880, 931, 1101, 1169, 1214 };
    int16_t sB1[] = { 344, 286, 361, 722, 781, 853, 982, 929, 990, 992 };
    int16_t* s = sG1;
    int32_t r, rankCount = 10;
    double minT = 300.0;
    double maxT = 3000.0;
    double deltaT = ( maxT - minT ) / double( rankCount - 1 );
    gkg::Kalman::Parameters parameters( 2 );
    gkg::Kalman* kalmanFilter = 0;
    double t = minT;

    switch ( signal )
    {

      case 2:
        s = sB1;
        break;

      case 1:
      default:
        s = sG1;
        break;

    }

    if ( type == "UKF" )
    {

      if ( kappa < 0.0 )
      {

        kappa = 0.01;

      }

      kalmanFilter = new gkg::UnscentedKalmanFilter( 
                  gkg::RCPointer< gkg::KFFunction >( new gkg::UKFFunctionT1() ),
                  kappa );
      parameters.X( 0 ) = 1000.0;
      parameters.X( 1 ) = 500.0;
      parameters.varianceOfX( 0, 0 ) = 1e5;
      parameters.varianceOfX( 1, 1 ) = 1e3;

    }
    else
    {

      kalmanFilter = new gkg::ExtendedKalmanFilter( 
                gkg::RCPointer< gkg::KFFunction >( new gkg::EKFFunctionT1() ) );
      parameters.X( 0 ) = 1000.0;
      parameters.X( 1 ) = 1000.0;

    }

    std::cout << std::setw( 6 ) << "Signal"
              << std::setw( 8 ) << "t"
              << std::setw( 6 ) << "y"
              << std::setw( 12 ) << "X0"
              << std::setw( 12 ) << "X1"
              << std::setw( 6 ) << "sgn"
              << std::setw( 12 ) << "S0"
              << std::setw( 12 ) << "T" << std::endl;
    std::cout << std::setw( 6 ) << s[ 0 ]
              << std::setw( 8 ) << t
              << std::setw( 6 ) << double( s[ 0 ] )
              << std::setw( 12 ) << parameters.X( 0 )
              << std::setw( 12 ) << parameters.X( 1 )
              << std::setw( 6 ) << 
                 ( ( t < ( parameters.X( 1 ) * std::log( 2.0 ) ) ) ? "-" : "+" )
              << std::setw( 12 ) << "---"
              << std::setw( 12 ) << "---"
              << std::endl;

    for ( r = 0; r < rankCount; r++, t += deltaT )
    {

      gkg::Vector R( 2 );

      R( 0 ) = 1.0;
      R( 1 ) = t;

      kalmanFilter->setSubDesignMatrix( R );

      double y = double( s[ r ] );

      kalmanFilter->iterate( parameters, y, r );

      double S0, T;

      S0 = std::max( std::min( parameters.X( 0 ), 4095.0 ), 0.0 );
      T = std::max( std::min( parameters.X( 1 ), 3000.0 ), 0.0 );

      std::cout << std::setw( 6 ) << s[ r ]
                << std::setw( 8 ) << t
                << std::setw( 6 ) << y
                << std::setw( 12 ) << parameters.X( 0 )
                << std::setw( 12 ) << parameters.X( 1 )
                << std::setw( 6 ) << 
                       ( ( t < ( parameters.X( 1 ) * std::log( 2.0 ) ) ) ? "-" 
                                                                         : "+" )
                << std::setw( 12 ) << S0
                << std::setw( 12 ) << T
                << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
