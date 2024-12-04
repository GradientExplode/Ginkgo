#include <gkg-qmri-plugin-functors/RelaxometryKalmanFilter/RelaxometryKalmanFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/ExtendedKalmanFilter.h>
#include <gkg-processing-numericalanalysis/UnscentedKalmanFilter.h>
#include <gkg-qmri-t1/EKFFunctionT1.h>
#include <gkg-qmri-t1/UKFFunctionT1.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>


gkg::RelaxometryKalmanFilterCommand::RelaxometryKalmanFilterCommand(
                                                              int32_t argc,
                                                              char* argv[], 
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                   : gkg::Command( argc, argv,
                                                   loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH(
         "gkg::RelaxometryKalmanFilterCommand::RelaxometryKalmanFilterCommand( "
         "int32_t argc, char* argv[], bool loadPlugin, bool removeFirst )" );

}


gkg::RelaxometryKalmanFilterCommand::RelaxometryKalmanFilterCommand(
                                  const std::string& fileNameIn, 
                                  const std::string& fileNameMask,
                                  const std::string& fileNameOut,
                                  const std::string& type,
                                  const std::vector< double >& scalarParameters,
                                  bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameMask, fileNameOut, 
             type, scalarParameters, verbose );

  }
  GKG_CATCH( 
         "gkg::RelaxometryKalmanFilterCommand::RelaxometryKalmanFilterCommand( "
         "const std::string& fileNameIn, const std::string& fileNameMask, "
         "const std::string& fileNameOut, const std::string& type, "
         "const std::vector< double >& scalarParameters, "
         "bool verbose )" );

}


gkg::RelaxometryKalmanFilterCommand::RelaxometryKalmanFilterCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, type );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           scalarParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameMask, fileNameOut, 
             type, scalarParameters, verbose );

  }
  GKG_CATCH( "gkg::RelaxometryKalmanFilterCommand::"
             "RelaxometryKalmanFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RelaxometryKalmanFilterCommand::~RelaxometryKalmanFilterCommand()
{
}


std::string gkg::RelaxometryKalmanFilterCommand::getStaticName()
{

  try
  {

    return "RelaxometryKalmanFilter";

  }
  GKG_CATCH( "std::string "
             "gkg::RelaxometryKalmanFilterCommand::getStaticName()" );

}


void gkg::RelaxometryKalmanFilterCommand::parse()
{

  try
  {

    std::string fileNameEPI;
    std::string fileNameMask;
    std::string fileNameOut;
    std::string type;
    std::vector< double > scalarParameters;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, 
                                  "Relaxometry Kalman's filter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input SE-EPI / GRE-EPI / IR-SE-EPI file name",
                                 fileNameEPI );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output base file name",
                                 fileNameOut );
    application.addSingleOption( "-type",
                                 "Mapping type t2 / t2star / t1",
                                 type );
    application.addSeriesOption( "-scalarParameters",
                                 "Scalar Parameters : \n"
                                 "- in case of T2 mapping : \n"
                                 "   <P1>: minimum TE value in ms\n"
                                 "   <P2>: maximum TE value in ms\n"
                                 "   <P3>: iteration count\n"
				 "- in case of T1 mapping : \n"
                                 "   <P1>: minimum TI value in ms\n"
                                 "   <P2>: maximum TI value in ms\n"
                                 "   <P3>: iteration count\n",
                                 scalarParameters,
                                 3 );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameEPI, fileNameMask, fileNameOut, 
             type, scalarParameters, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RelaxometryKalmanFilterCommand::parse()" );

}


void gkg::RelaxometryKalmanFilterCommand::execute(
                                  const std::string& fileNameIn, 
                                  const std::string& fileNameMask,
                                  const std::string& fileNameOut,
                                  const std::string& type,
                                  const std::vector< double >& scalarParameters,
                                  bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< int16_t > epi;
    gkg::Reader::getInstance().read( fileNameIn, epi );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t sizeX = epi.getSizeX();
    int32_t sizeY = epi.getSizeY();
    int32_t sizeZ = epi.getSizeZ();
    int32_t rankCount = epi.getSizeT();

    gkg::Vector3d< double > resolution;
    epi.getResolution( resolution );
  

    ////////////////////////////////////////////////////////////////////////////
    // reading mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Volume< int16_t > mask;
    gkg::Reader::getInstance().read( fileNameMask, mask );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    gkg::Kalman* kalmanFilter = 0;

    if ( ( type == "t2" ) || ( type == "t2star" ) )
    {

      kalmanFilter = new gkg::Kalman( 
                gkg::RCPointer< gkg::KFFunction >( new gkg::KFFunction( 2 ) ) );

    }
    else
    {

      kalmanFilter = new gkg::ExtendedKalmanFilter( 
                gkg::RCPointer< gkg::KFFunction >( new gkg::EKFFunctionT1() ) );
      //kalmanFilter = new gkg::UnscentedKalmanFilter( 
      //        gkg::RCPointer< gkg::KFFunction >( new gkg::UKFFunctionT1() ) );

    }
      
    if ( !kalmanFilter )
    {
    
      throw std::runtime_error( "No kalman filter initialized" );
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // initializing Kalman's filter structure
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > S0( sizeX, sizeY, sizeZ, rankCount );
    gkg::Volume< float > T( sizeX, sizeY, sizeZ, rankCount );

    S0.setResolution( resolution );
    T.setResolution( resolution );
    S0.fill( 0.0 );
    T.fill( 0.0 );

    gkg::Volume< gkg::Kalman::Parameters* > kalmanLut( sizeX, 
                                                       sizeY, 
                                                       sizeZ );
    kalmanLut.getHeader().addAttribute( "resolutionX", resolution.x );
    kalmanLut.getHeader().addAttribute( "resolutionY", resolution.y );
    kalmanLut.getHeader().addAttribute( "resolutionZ", resolution.z );
    kalmanLut.fill( 0 );

    int32_t x, y, z, r;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) && ( epi( x, y, z ) > 0 ) )
          {

	    gkg::Kalman::Parameters* parameters =
                                               new gkg::Kalman::Parameters( 2 );

            if ( type == "t1" )
            {

              parameters->X( 0 ) = 1000.0;
              parameters->X( 1 ) = 1000.0;

            }

            kalmanLut( x, y, z ) = parameters;

          }

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // Kalman's filter iterations
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "performing Kalman iterations : " << std::flush;

    }

    for ( r = 0; r < rankCount; r++ )
    {

      if ( verbose )
      {

        if ( r )
        {

          std::cout << gkg::Eraser( 22 );

        }
        std::cout << " [ " << std::setw( 7 ) << r + 1 
                  << " / " << std::setw( 7 ) << rankCount
                  << " ]" << std::flush;

      }

      gkg::Vector R( 2 );
      R( 0 ) = 1.0;
      R( 1 ) = scalarParameters[ 0 ] + 
               ( double )r * ( scalarParameters[ 1 ] - scalarParameters[ 0 ] ) /
               ( ( double )rankCount - 1.0 );

      kalmanFilter->setSubDesignMatrix( R );

      if ( ( type == "t2" ) || ( type == "t2star" ) )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              gkg::Kalman::Parameters* parameters = kalmanLut( x, y, z );

              if ( parameters )
              {

                kalmanFilter->iterate( *parameters,
                                       std::log( ( double )epi( x, y, z, r ) ),
                                       r );
                gkg::Vector& X = parameters->X;
                S0( x, y, z, r ) = std::max( std::min( std::exp( X( 0 ) ),
                                             4095.0 ),
                                             0.0 );
                T( x, y, z, r ) = std::max( std::min( -1.0 / X( 1 ), 500.0 ),
                                            0.0 );

              }

            }

          }

        }

      }
      else
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              gkg::Kalman::Parameters* parameters = kalmanLut( x, y, z );

              if ( parameters )
              {

                kalmanFilter->iterate( *parameters,
                                       ( double )epi( x, y, z, r ),
                                       r );
                gkg::Vector& X = parameters->X;
                S0( x, y, z, r ) = std::max( std::min( X( 0 ), 4095.0 ), 0.0 );
                T( x, y, z, r ) = std::max( std::min( X( 1 ), 3000.0 ), 0.0 );

              }

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building fitted EPI data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building fitted EPI data : " << std::flush;

    }
    gkg::Volume< float > fitted( sizeX, sizeY, sizeZ, rankCount );
    fitted.setResolution( resolution );
    fitted.fill( 0.0 );
    float ti = float( scalarParameters[ 0 ] );
    float deltaTI = ( scalarParameters[ 1 ] - scalarParameters[ 0 ] ) /
                    ( ( double )rankCount - 1.0 ) ;

    for ( r = 0; r < rankCount; r++, ti += deltaTI )
    {

      if ( ( type == "t2" ) || ( type == "t2star" ) )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( kalmanLut( x, y, z ) )
              {

                fitted( x, y, z, r ) = S0( x, y, z, rankCount - 1 ) *
                                       std::exp( -ti /
                                                 T( x, y, z, rankCount - 1 ) );

              }

            }

          }

        }

      }
      else
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( kalmanLut( x, y, z ) )
              {

                fitted( x, y, z, r ) = std::fabs( S0( x, y, z, rankCount - 1 ) *
                                       ( 1.0 - 2.0 * std::exp( -ti /
                                              T( x, y, z, rankCount - 1 ) ) ) );

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
  

    ////////////////////////////////////////////////////////////////////////////
    // saving results
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving results : " << std::flush;

    }

    std::string ext = "_T2";

    if ( type == "t1" )
    {

      ext = "_T1";

    }
    else if ( type == "t2star" )
    {

      ext = "_T2star";

    }

    gkg::Writer::getInstance().write( fileNameOut + "_S0", S0 );
    gkg::Writer::getInstance().write( fileNameOut + ext, T );
    gkg::Writer::getInstance().write( fileNameOut + "_fitted", fitted );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // deleting Kalman loopkup table items
    ////////////////////////////////////////////////////////////////////////////

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          delete kalmanLut( x, y, z );

        }

      }

    }

    delete kalmanFilter;

  }
  GKG_CATCH( "void gkg::RelaxometryKalmanFilterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameMask, "
             "const std::string& fileNameOut, const std::string& type, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


RegisterCommandCreator(
                RelaxometryKalmanFilterCommand,
    		DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    		DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    		DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		DECLARE_STRING_PARAMETER_HELP( type ) +
    		DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    		DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
