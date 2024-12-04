#include <gkg-processing-plugin-functors/PhilipsPhaseMapConverter/PhilipsPhaseMapConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::PhilipsPhaseMapConverterCommand::PhilipsPhaseMapConverterCommand(
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
  GKG_CATCH( "gkg::PhilipsPhaseMapConverterCommand::PhilipsPhaseMapConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::PhilipsPhaseMapConverterCommand::PhilipsPhaseMapConverterCommand(
                                     		const std::string& fileNameIn,
                                     		const std::string& fileNameOut,
                                     		const std::string& outputFormat,
                                     		bool ascii,
                                     		bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH( 
       "gkg::PhilipsPhaseMapConverterCommand::PhilipsPhaseMapConverterCommand( "
       "const std::string& fileNameIn, const std::string& fileNameOut, "
       "const std::string& outputFormat, bool ascii, bool verbose )" );

}


gkg::PhilipsPhaseMapConverterCommand::PhilipsPhaseMapConverterCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH( "gkg::PhilipsPhaseMapConverterCommand::"
             "PhilipsPhaseMapConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::PhilipsPhaseMapConverterCommand::~PhilipsPhaseMapConverterCommand()
{
}


std::string gkg::PhilipsPhaseMapConverterCommand::getStaticName()
{

  try
  {

    return "PhilipsPhaseMapConverter";

  }
  GKG_CATCH( "std::string gkg::PhilipsPhaseMapConverterCommand::getStaticName()" );

}


void gkg::PhilipsPhaseMapConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Philips int16_t to float "
                                  "phase map converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input int16_t SIEMENS phase map",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output float phase map",
                                 fileNameOut );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::PhilipsPhaseMapConverterCommand::parse()" );

}


void gkg::PhilipsPhaseMapConverterCommand::execute(
                  	             		const std::string& fileNameIn,
                                     		const std::string& fileNameOut,
                                     		const std::string& outputFormat,
                                     		bool ascii,
                                     		bool verbose )
{

  try
  {

    // sanity checks
    if ( fileNameOut == fileNameIn )
    {

      throw std::runtime_error(
                               "input and ouput file names must be different" );

    }

    // reading input proxy
    if ( verbose )
    {

      std::cout << "reading PHILIPS phase map '"
                << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< float > inputVolume;
    gkg::TypedVolumeReaderProcess< float > volumeReaderProcess( inputVolume );
    volumeReaderProcess.execute( fileNameIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // converting 
    if ( verbose )
    {

      std::cout << "converting -pi -> +pi : " << std::flush;

    }
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();
    int32_t sizeT = inputVolume.getSizeT();

    // Sanity check
    if ( sizeT != 1 )
    {

      throw std::runtime_error( "Error: input sizeT should be equal to 1 !" );

    }


    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( inputVolume.getHeader().hasAttribute( "resolutionX" ) )
    {

       inputVolume.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( inputVolume.getHeader().hasAttribute( "resolutionY" ) )
    {

       inputVolume.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( inputVolume.getHeader().hasAttribute( "resolutionZ" ) )
    {

       inputVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }


    gkg::MinMaxFilter< gkg::Volume< float >, float > minMaxFilter;
    std::pair< float, float > minMax;

    minMaxFilter.filter( inputVolume, minMax );

    gkg::Volume< float > outputVolume( sizeX, sizeY, sizeZ );
    outputVolume.getHeader().addAttribute( "resolutionX", resolutionX );
    outputVolume.getHeader().addAttribute( "resolutionY", resolutionY );
    outputVolume.getHeader().addAttribute( "resolutionZ", resolutionZ );

    gkg::Volume< float >::const_iterator i = inputVolume.begin(),
                                         ie = inputVolume.end();
    gkg::Volume< float >::iterator o = outputVolume.begin();
    while ( i != ie )
    {

      *o = ( ( ( float )*i - minMax.first ) /
             ( minMax.second - minMax.first ) ) * 2 * M_PI - M_PI;
      ++ i;
      ++ o;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // writing float output
    if ( verbose )
    {

      std::cout << "writing float volume '" << fileNameOut << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      outputVolume,
                                      ascii,
                                      outputFormat );
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PhilipsPhaseMapConverterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool ascii, bool verbose )" );

}


RegisterCommandCreator( PhilipsPhaseMapConverterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
