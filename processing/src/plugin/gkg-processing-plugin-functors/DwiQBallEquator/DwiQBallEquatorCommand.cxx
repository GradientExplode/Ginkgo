#include <gkg-processing-plugin-functors/DwiQBallEquator/DwiQBallEquatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <algorithm>


gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand( int32_t argc,
                                 		     char* argv[],
                                 		     bool loadPlugin,
                                 		     bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand(                           
                             const std::vector< double >& normalizedOrientation,
    			     int32_t pointCount,
    			     const std::string& fileNameOut,
    			     double bValue,
    			     bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( normalizedOrientation, pointCount, fileNameOut, bValue, verbose );

  }
  GKG_CATCH( "gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand( "
             "const std::vector< double >& normalizedOrientation, "
    	     "int32_t pointCount, const std::string& fileNameOut, "
    	     "double bValue, bool verbose )" );

}


gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           normalizedOrientation );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, pointCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, double, bValue );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( normalizedOrientation, pointCount, fileNameOut, bValue, verbose );

  }
  GKG_CATCH( "gkg::DwiQBallEquatorCommand::DwiQBallEquatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQBallEquatorCommand::~DwiQBallEquatorCommand()
{
}


std::string gkg::DwiQBallEquatorCommand::getStaticName()
{

  try
  {

    return "DwiQBallEquator";

  }
  GKG_CATCH( "std::string gkg::DwiQBallEquatorCommand::getStaticName()" );

}


void gkg::DwiQBallEquatorCommand::parse()
{

  try
  {

    std::vector< double > normalizedOrientation;
    int32_t pointCount;
    std::string fileNameOut;
    double bValue = -1.0;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Process coordinates of points on the equator"
                                  " of a given orientation",
                                  _loadPlugin );
    application.addSeriesOption( "-orientation",
                                 "Normalized orientation",
                                 normalizedOrientation,
                                 3, 3 );
    application.addSingleOption( "-c",
                                 "Point count on the equator",
                                 pointCount );
    application.addSingleOption( "-o",
                                 "Output text file",
                                 fileNameOut );
    application.addSingleOption( "-b",
                                 "Add b-value at the end of each point",
                                 bValue,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( normalizedOrientation, pointCount, fileNameOut, bValue, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQBallEquatorCommand::parse()" );

}


void gkg::DwiQBallEquatorCommand::execute(
                  	     const std::vector< double >& normalizedOrientation,
    			     int32_t pointCount,
    			     const std::string& fileNameOut,
    			     double bValue,
    			     bool verbose )
{

  try
  {

    // sanity checks
    if ( pointCount <= 0 )
    {

      throw std::runtime_error( "bad point count on the equator" );

    }

    // normalizing orientation
    gkg::Vector3d< double > orientation( normalizedOrientation[ 0 ],
                                         normalizedOrientation[ 1 ],
                                         normalizedOrientation[ 2 ] );
    orientation.normalize();
    if ( verbose )
    {

      std::cout << "normalized orientation : " << orientation << std::endl;

    }

    if ( verbose )
    {

      std::cout << "preparing '" << fileNameOut << "' file : " << std::flush;

    }
    std::ofstream os( fileNameOut.c_str() );
    os << pointCount << std::endl;
    if ( verbose )
    {

      std::cout << "done" << std::endl;;

    }


    if ( verbose )
    {

      std::cout << "processing equator points : " << std::flush;

    }

    gkg::Vector3d< double > zaxis( 0.0, 0.0, 1.0 );
    gkg::Vector3d< double > point;
    int32_t p;
    for ( p = 0; p < pointCount; p++ )
    {

      // building the pth circle point coordinates
      double theta = 2.0 * M_PI * p / ( double )pointCount;
      double ctheta = std::cos( theta );
      double stheta = std::sin( theta );

      if ( orientation != zaxis )
      {

        point.x = ( ( orientation.x * orientation.x ) /
                    ( 1 + orientation.z ) - 1 ) * ctheta +
                    ( orientation.x * orientation.y /
                    ( 1 + orientation.z ) ) * stheta;
        point.y = ( ( orientation.x * orientation.y ) /
                    ( 1 + orientation.z ) ) * ctheta +
                  ( ( orientation.y * orientation.y ) /
                    ( 1 + orientation.z ) - 1 ) * stheta;
        point.z = orientation.x * ctheta + orientation.y * stheta;

      }
      else
      {

        point.x = ctheta;
        point.y = stheta;
        point.z = 0.0;

      }

      os << point.x << "\t"
         << point.y << "\t"
         << point.z;

      if ( bValue > 0.0 )
      {

        os << "\t" << bValue;

      }

      os << std::endl;

    }

    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiQBallEquatorCommand::execute( "
             "const std::vector< double >& normalizedOrientation, "
    	     "int32_t pointCount, const std::string& fileNameOut, "
    	     "double bValue, bool verbose )" );

}


RegisterCommandCreator(
           DwiQBallEquatorCommand,
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( normalizedOrientation ) +
           DECLARE_INTEGER_PARAMETER_HELP( pointCount ) +
           DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
           DECLARE_FLOATING_PARAMETER_HELP( bValue ) +
           DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
