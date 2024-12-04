#include <gkg-simulation-plugin-functors/RotateAndAddNoiseToMedusaMRISignature/RotateAndAddNoiseToMedusaMRISignatureCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>



gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::
                                   RotateAndAddNoiseToMedusaMRISignatureCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                                  : gkg::Command( argc, argv,
                                                                  loadPlugin,
                                                                  removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::"
             "RotateAndAddNoiseToMedusaMRISignatureCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::
                                   RotateAndAddNoiseToMedusaMRISignatureCommand(
                            const std::string& fileNameInputMRISignature,
                            const std::string& directoryNameOutputMRISignatures,
                            int32_t rotationCount,
                            float noiseStandardDeviation,
                            bool verbose )
{

  try
  {

    execute( fileNameInputMRISignature,
             directoryNameOutputMRISignatures,
             rotationCount,
             noiseStandardDeviation,
             verbose );

  }
  GKG_CATCH( "gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::"
             "RotateAndAddNoiseToMedusaMRISignatureCommand( "
             "const std::string& fileNameInputMRISignature, "
             "const std::string& directoryNameOutputMRISignatures, "
             "int32_t rotationCount, "
             "float noiseStandardDeviation, "
             "bool verbose )" );

}


gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::
                                   RotateAndAddNoiseToMedusaMRISignatureCommand(
                                             const gkg::Dictionary& parameters )
                                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameInputMRISignature );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              directoryNameOutputMRISignatures );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rotationCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, noiseStandardDeviation );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputMRISignature,
             directoryNameOutputMRISignatures,
             rotationCount,
             noiseStandardDeviation,
             verbose );

  }
  GKG_CATCH( "gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::"
             "RotateAndAddNoiseToMedusaMRISignatureCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::
                                 ~RotateAndAddNoiseToMedusaMRISignatureCommand()
{
}


std::string gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::getStaticName()
{

  try
  {

    return "RotateAndAddNoiseToMedusaMRISignature";

  }
  GKG_CATCH( "std::string "
             "gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::"
             "getStaticName()" );

}


void gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::parse()
{

  try
  {

    std::string fileNameInputMRISignature;
    std::string directoryNameOutputMRISignatures;
    int32_t rotationCount = 0;
    float noiseStandardDeviation = 0.0f;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                  "Tool to create rotated and noise-added MEDUSA MRI signatures"
                  " from a noise-free input MEDUSA MRI signature",
                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input MEDUSA Q-space encoded MRI signature "
                                 "filename",
                                 fileNameInputMRISignature );
    application.addSingleOption( "-o",
                                 "Output directory name for rotated and "
                                 "noise-corrupted MEDUSA MRI signatures",
                                 directoryNameOutputMRISignatures );
    application.addSingleOption( "-r",
                                 "Rotation count",
                                 rotationCount );
    application.addSingleOption( "-n",
                                 "Noise standard deviation betwen 0.0 and 1.0",
                                 noiseStandardDeviation );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameInputMRISignature,
             directoryNameOutputMRISignatures,
             rotationCount,
             noiseStandardDeviation,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::"
                     "parse()" );

}


void gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::execute(
                            const std::string& fileNameInputMRISignature,
                            const std::string& directoryNameOutputMRISignatures,
                            int32_t rotationCount,
                            float noiseStandardDeviation,
                            bool verbose )
{

  try
  {

    //
    // reading input MEDUSA MRI signature
    //
    if ( verbose )
    {

      std::cout << "reading input MEDUSA MRI signature : " << std::flush;

    }

    // collecting input MEDUSA MRI signature basename
    gkg::File file( fileNameInputMRISignature );
    std::string baseName = file.getBaseName();

    // allocating MEDUSA MRI signature volume
    gkg::Volume< float > inputMedusaMRISignature;

    // reading from disk
    gkg::Reader::getInstance().read( fileNameInputMRISignature,
                                     inputMedusaMRISignature );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // decoding Q-space sampling
    //
    if ( verbose )
    {

      std::cout << "decoding Q-space sampling : " << std::endl;

    }

    gkg::SingleShellQSpaceSampling*
      qSpaceSampling = static_cast< gkg::SingleShellQSpaceSampling* >(
                           gkg::QSpaceSamplingFactory::getInstance().create(
                                                    inputMedusaMRISignature ) );
    int32_t orientationCount = qSpaceSampling->getOrientationSet().getCount();
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "b-value : " << qSpaceSampling->getBValue() << std::endl;
      std::cout << "orientation count : " << orientationCount << std::endl;

    }

    delete qSpaceSampling;

    //
    // generating and writing rotated and noise-corrupted MEDUSA MRI signatures
    //

    if ( verbose )
    {

      std::cout << "generating rotation and rotated noise-corrupted MEDUSA "
                << "MRI signature(s) : "
                << std::flush;

    }

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    // allocating a random generator
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // resetting random generator seed with current time
    randomGenerator.setSeedWithTime();

    std::string maximumIndexToString = gkg::StringConverter::toString(
                                                            rotationCount + 1 );
    size_t s = 0U;
    for ( s = 0U; s < maximumIndexToString.size(); s++ )
    {

      maximumIndexToString[ s ] = '0';

    }

    double realNoise = 0.0;
    double imagNoise = 0.0;
    int32_t rotationIndex = 0;
    for ( rotationIndex = 0; rotationIndex < rotationCount; rotationIndex++ )
    {

      // creating output MEDUSA signature
      gkg::Volume< float > outputMedusaMRISignature( inputMedusaMRISignature );

      // corrupting signature with noise
      gkg::Volume< float >::const_iterator
        is = inputMedusaMRISignature.begin(),
        ise = inputMedusaMRISignature.end();
      gkg::Volume< float >::iterator
        os = outputMedusaMRISignature.begin();
      while ( is != ise )
      {

        realNoise = factory->getUniformRandomNumber(
                                        randomGenerator,
                                        0.0, ( double )noiseStandardDeviation );
        imagNoise = factory->getUniformRandomNumber(
                                        randomGenerator,
                                        0.0, ( double )noiseStandardDeviation );
        *os = ( float )std::sqrt( ( ( double )*is + realNoise ) *
                                  ( ( double )*is + realNoise ) +
                                  ( imagNoise * imagNoise ) );
        ++ is;
        ++ os;

      }

      // rotating output Q-space sampling
      gkg::SingleShellQSpaceSampling*
        qSpaceSampling = static_cast< gkg::SingleShellQSpaceSampling* >(
                           gkg::QSpaceSamplingFactory::getInstance().create(
                                                   outputMedusaMRISignature ) );

      gkg::Vector3d< float > rotation( 
          factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ),
          factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ),
          factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ) );
      int32_t o = 0;
      for ( o = 0; o < orientationCount; o++ )
      {

        qSpaceSampling->setMotionRotation3d( o, rotation );

      }

      qSpaceSampling->addInformationToHeader( outputMedusaMRISignature );

      delete qSpaceSampling;
      

      // writing output signature
      std::string indexToString = gkg::StringConverter::toString(
                                                            rotationIndex + 1 );

      indexToString = maximumIndexToString.substr( 0, 
                        maximumIndexToString.size() - indexToString.size() ) +
                      indexToString;

      gkg::Directory outputDirectory( directoryNameOutputMRISignatures + 
                                      gkg::getDirectorySeparator() +
                                      "RotationNumber_" +
                                      indexToString );
      outputDirectory.mkdir();
      std::string fileNameOutputMRISignature = outputDirectory.getPath() +
                                               gkg::getDirectorySeparator() +
                                               baseName;

      gkg::Writer::getInstance().write( fileNameOutputMRISignature,
                                        outputMedusaMRISignature );

      // writing output rotation
      gkg::Rotation3d< float > motionRotation3d( rotation );

      std::string fileNameOutputRotation3d = outputDirectory.getPath() +
                                               gkg::getDirectorySeparator() +
                                               "rotation3d.py";

      std::vector< float > coefficients = motionRotation3d.getComposition().
                                                        getDirectCoefficients();
      std::ofstream ofs( fileNameOutputRotation3d.c_str() );
      ofs << "[ [ " << coefficients[ 0 ] << ", "
                    << coefficients[ 1 ] << ", "
                    << coefficients[ 2 ] << " ], [ "
                    << coefficients[ 4 ] << ", "
                    << coefficients[ 5 ] << ", "
                    << coefficients[ 6 ] << " ], [ "
                    << coefficients[ 8 ] << ", "
                    << coefficients[ 9 ] << ", "
                    << coefficients[ 10 ] << " ] ]" << std::endl;
      ofs.close();
                   

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::RotateAndAddNoiseToMedusaMRISignatureCommand::execute( "
             "const std::string& fileNameInputMRISignature, "
             "const std::string& directoryNameOutputMRISignatures, "
             "int32_t rotationCount, "
             "float noiseStandardDeviation, "
             "bool verbose )" );

}


RegisterCommandCreator(
             RotateAndAddNoiseToMedusaMRISignatureCommand,
    	     DECLARE_STRING_PARAMETER_HELP( fileNameInputMRISignature ) +
    	     DECLARE_STRING_PARAMETER_HELP( directoryNameOutputMRISignatures ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( rotationCount ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( noiseStandardDeviation ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

