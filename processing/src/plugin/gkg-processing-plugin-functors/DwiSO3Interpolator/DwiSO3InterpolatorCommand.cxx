#include <gkg-processing-plugin-functors/DwiSO3Interpolator/DwiSO3InterpolatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>


gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand(
                                		const std::string& fileNameIn,
    			        		const std::string& fileNameOut,
    			        		int32_t outputOrientationCount,
    			        		int32_t maximumNeighborCount,
    			        		float sigma,
    			        		bool ascii,
    			        		const std::string& outputFormat,
    			        		bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputOrientationCount,
             maximumNeighborCount, sigma, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "int32_t outputOrientationCount, int32_t maximumNeighborCount, "
    	     "float sigma, bool ascii, const std::string& outputFormat, "
    	     "bool verbose )" );

}


gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumNeighborCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, sigma );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outputOrientationCount,
             maximumNeighborCount, sigma, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::DwiSO3InterpolatorCommand::DwiSO3InterpolatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSO3InterpolatorCommand::~DwiSO3InterpolatorCommand()
{
}


std::string gkg::DwiSO3InterpolatorCommand::getStaticName()
{

  try
  {

    return "DwiSO3Interpolator";

  }
  GKG_CATCH( "std::string gkg::DwiSO3InterpolatorCommand::getStaticName()" );

}


void gkg::DwiSO3InterpolatorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t outputOrientationCount;
    int32_t maximumNeighborCount = 10;
    float sigma = 1.0;
    bool ascii = false;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Remove diffusion gradient orientations from"
                                  " diffusion data",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input diffusion-weighted data file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output diffusion-weighted data file name",
                                 fileNameOut );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count",
                                 outputOrientationCount );
    application.addSingleOption( "-maximumNeighborCount",
                                 "Maximum orientation neighbor count "
                                 "(default=10)",
                                 maximumNeighborCount,
                                 true );
    application.addSingleOption( "-sigma",
                                 "Geodesic SO3 interpolator std deviation"
                                 "(default=1.0)",
                                 sigma,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, outputOrientationCount,
             maximumNeighborCount, sigma, ascii, outputFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiSO3InterpolatorCommand::parse()" );

}


void gkg::DwiSO3InterpolatorCommand::execute(
                                          const std::string& fileNameIn,
    			        	  const std::string& fileNameOut,
    			        	  int32_t outputOrientationCount,
    			        	  int32_t maximumNeighborCount,
    			        	  float sigma,
    			        	  bool /* ascii */,
    			        	  const std::string& /* outputFormat */,
    			        	  bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////
    if ( outputOrientationCount < 6 )
    {

      throw std::runtime_error( 
                               "sub-set orientation count must be at least 6" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input proxy
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : "
                << std::flush;

    }
    gkg::Volume< int16_t > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting size information
    ////////////////////////////////////////////////////////////////////////////
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();
    int32_t inputOrientationCount = inputVolume.getSizeT();


    ////////////////////////////////////////////////////////////////////////////
    // collecting old orientation set
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "collecting input orientation set : "
                << std::flush;

    }
    gkg::GenericObjectList 
      inputGenericObjectListOrientations( inputOrientationCount );
    std::vector< gkg::Vector3d< float > >
      inputOrientationVector( inputOrientationCount );
    if ( inputVolume.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      inputVolume.getHeader().getAttribute( "diffusion_gradient_orientations",
                                           inputGenericObjectListOrientations );
      int32_t index = 0;
      for ( index = 0; index < inputOrientationCount; index++ )
      {

        gkg::GenericObjectList
          tmp = inputGenericObjectListOrientations[ index ]->
                                           getValue< gkg::GenericObjectList >();
        inputOrientationVector[ index ].x = ( float )tmp[ 0 ]->getScalar();
        inputOrientationVector[ index ].y = ( float )tmp[ 1 ]->getScalar();
        inputOrientationVector[ index ].z = ( float )tmp[ 2 ]->getScalar();

      }

    }
    gkg::OrientationSet inputOrientationSet( inputOrientationVector );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building output orientation set
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building output orientation set : "
                << std::flush;

    }
    gkg::ElectrostaticOrientationSet 
      outputOrientationSet( outputOrientationCount );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building geodesic weight map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building geodesic SO3 interpolation weights : "
                << std::flush;

    }
    std::vector< std::vector< std::pair< int32_t, float > > >
      indicesAndWeights( outputOrientationCount );
    gkg::GeodesicSO3Interpolator geodesicSO3Interpolator( maximumNeighborCount,
                                                          sigma );
    int32_t o = 0;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      indicesAndWeights[ o ] =
        geodesicSO3Interpolator.getNeighborIndicesAndWeights(
                                     inputOrientationSet,
                                     outputOrientationSet.getOrientation( o ) );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output orientation/b-value information(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building output b-value/orientation header information : "
                << std::flush;

    }
    std::vector< double > inputBValues( inputOrientationCount );
    double bValue = 0.0;
    if ( inputVolume.getHeader().hasAttribute( "bvalues" ) )
    {

      inputVolume.getHeader().getAttribute( "bvalues", inputBValues );
      bValue = inputBValues[ 0 ];
      int32_t index = 0;
      for ( index = 1; index < inputOrientationCount; index++ )
      {

        if ( inputBValues[ index ] != bValue )
        {

          throw std::runtime_error( "different b-values on the shell" );

        }

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'bvalues'" );

    }
    std::vector< double > outputBValues( outputOrientationCount, bValue );

    gkg::GenericObjectList
      outputGenericObjectListOrientations( outputOrientationCount );
    std::vector< double > orientation( 3 );
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      orientation[ 0 ] = outputOrientationSet.getOrientation( o ).x;
      orientation[ 1 ] = outputOrientationSet.getOrientation( o ).y;
      orientation[ 2 ] = outputOrientationSet.getOrientation( o ).z;
      outputGenericObjectListOrientations[ o ] = orientation;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output volume
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "interpolating output volume : "
                << std::flush;

    }
    gkg::Volume< int16_t > outputVolume( sizeX,
                                         sizeY,
                                         sizeZ,
                                         outputOrientationCount );
    outputVolume.getHeader() = inputVolume.getHeader();
    outputVolume.getHeader()[ "sizeT" ] = outputOrientationCount;
    outputVolume.getHeader()[ "bvalues" ] = outputBValues;
    outputVolume.getHeader()[ "diffusion_gradient_orientations" ] =
                                            outputGenericObjectListOrientations;

    int x, y, z, n;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            float value = 0.0;
            for ( n = 0; n < maximumNeighborCount; n++ )
            {

              value +=
                indicesAndWeights[ o ][ n ].second *
                inputVolume( x, y, z, indicesAndWeights[ o ][ n ].first );

            }
            outputVolume( x, y, z, o ) = ( int16_t )value;

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output volume
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, outputVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiSO3InterpolatorCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "int32_t outputOrientationCount, int32_t maximumNeighborCount, "
    	     "float sigma, bool ascii, const std::string& outputFormat, "
    	     "bool verbose )" );

}


RegisterCommandCreator(
                      DwiSO3InterpolatorCommand,
                      DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                      DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                      DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
                      DECLARE_INTEGER_PARAMETER_HELP( maximumNeighborCount ) +
                      DECLARE_FLOATING_PARAMETER_HELP( sigma ) +
                      DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                      DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
                      DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
