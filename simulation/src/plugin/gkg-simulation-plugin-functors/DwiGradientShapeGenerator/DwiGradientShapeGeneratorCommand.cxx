#include <gkg-simulation-plugin-functors/DwiGradientShapeGenerator/DwiGradientShapeGeneratorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-simulation-mri-sequence/MRISequencePool.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>


gkg::DwiGradientShapeGeneratorCommand::DwiGradientShapeGeneratorCommand(
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
  GKG_CATCH( "gkg::DwiGradientShapeGeneratorCommand::"
             "DwiGradientShapeGeneratorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiGradientShapeGeneratorCommand::DwiGradientShapeGeneratorCommand(
                const std::string& fileNameMRISequenceInputDictionary,
                const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                float timeStepInUs,
                bool verbose )
{

  try
  {

    execute( fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             timeStepInUs,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGradientShapeGeneratorCommand::"
             "DwiGradientShapeGeneratorCommand( "
             "const std::string& fileNameMRISequenceInputDictionary, "
             "const std::string& "
             "directoryNameMRISequenceOutputNormalizedGradientProfiles, "
             "bool verbose )" );

}


gkg::DwiGradientShapeGeneratorCommand::DwiGradientShapeGeneratorCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMRISequenceInputDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                     directoryNameMRISequenceOutputNormalizedGradientProfiles );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeStepInUs );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             timeStepInUs,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGradientShapeGeneratorCommand::"
             "DwiGradientShapeGeneratorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiGradientShapeGeneratorCommand::~DwiGradientShapeGeneratorCommand()
{
}


std::string gkg::DwiGradientShapeGeneratorCommand::getStaticName()
{

  try
  {

    return "DwiGradientShapeGenerator";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiGradientShapeGeneratorCommand::"
             "getStaticName()" );

}


void gkg::DwiGradientShapeGeneratorCommand::parse()
{

  try
  {

    std::string fileNameMRISequenceInputDictionary;
    std::string directoryNameMRISequenceOutputNormalizedGradientProfiles;
    float timeStepInUs = 10.0;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "OGSE gradient profile generator",
                                  _loadPlugin );
    application.addSingleOption( "-mriSequenceInputDictionary",
                                 "MRI sequence(s) dictionary "
                                 "file name",
                                 fileNameMRISequenceInputDictionary );
    application.addSingleOption(
                     "-mriSequenceOutputNormalizedGradientProfileDirectory",
                     "MRI sequence(s) directory name where to store "
                     "normalized gradient profiles (empty=not stored)",
                     directoryNameMRISequenceOutputNormalizedGradientProfiles );
    application.addSingleOption( "-timeStepInUs",
                                 "Time step in us (default=10.0us)",
                                 timeStepInUs, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             timeStepInUs,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiGradientShapeGeneratorCommand::parse()" );

}


void gkg::DwiGradientShapeGeneratorCommand::execute(
    const std::string& fileNameMRISequenceInputDictionary,
    const std::string& directoryNameMRISequenceOutputNormalizedGradientProfiles,
    float timeStepInUs,
    bool verbose )
{

  try
  {

    //
    // creating MRI sequence pool of sequence(s) on demand
    //
    if ( verbose )
    {

      std::cout << "reading MRI sequence input dictionary : " << std::flush;

    }

    gkg::UncommentCounterInputFileStream
      mriSequenceUcis( fileNameMRISequenceInputDictionary.c_str() );
    gkg::BaseObjectReader mriSequenceBaseObjectReader;
    gkg::TypedObject< gkg::Dictionary > mriSequenceDictionary;
    mriSequenceBaseObjectReader.read( mriSequenceUcis, mriSequenceDictionary );
    mriSequenceUcis.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // creating MRI sequence pool
    //
    if ( verbose )
    {

      std::cout << "creating MRI sequence pool : " << std::endl;

    }

    gkg::RCPointer< gkg::MRISequencePool >
      mriSequencePool(
        new gkg::MRISequencePool(
                          mriSequenceDictionary.getTypedValue(),
                          timeStepInUs,
                          verbose ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // writing MRI sequence profile(s)
    //

    if ( verbose )
    {

      std::cout << "writing MRI sequence profile(s) : " << std::flush;

    }
    mriSequencePool->saveNormalizedGradientProfiles(
             directoryNameMRISequenceOutputNormalizedGradientProfiles );

    int32_t mriSequenceCount = mriSequencePool->getCount();
    int32_t mriSequenceIndex = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      const gkg::RCPointer< gkg::MRISequence >&
         mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );
      mriSequence->saveDiffusionInformation(
                directoryNameMRISequenceOutputNormalizedGradientProfiles + "/" +
                mriSequencePool->getMRISequenceName( mriSequenceIndex ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    
  }
  GKG_CATCH( "void gkg::DwiGradientShapeGeneratorCommand::execute( "
             "const std::string& fileNameMRISequenceInputDictionary, "
             "const std::string& "
             "directoryNameMRISequenceOutputNormalizedGradientProfiles, "
             "float timeStepInUs, "
             "bool verbose )" );

}


RegisterCommandCreator(
             DwiGradientShapeGeneratorCommand,
    	     DECLARE_STRING_PARAMETER_HELP(
                                          fileNameMRISequenceInputDictionary ) +
    	     DECLARE_STRING_PARAMETER_HELP(
                    directoryNameMRISequenceOutputNormalizedGradientProfiles ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( timeStepInUs ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

