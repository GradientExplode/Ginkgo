#include <gkg-processing-plugin-functors/GehcPhaseMapConverter/GehcPhaseMapConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <complex>


float wrapPhase( const float& value )
{

  float r = value;
  while ( r > M_PI )
  {

    r -= 2 * M_PI;

  }
  while ( r < -M_PI )
  {

    r += 2 * M_PI;

  }
  return r;

}


gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin,
                                 bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand(
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
  GKG_CATCH( "gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool ascii, bool verbose )" );

}


gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand(
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
  GKG_CATCH( "gkg::GehcPhaseMapConverterCommand::GehcPhaseMapConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcPhaseMapConverterCommand::~GehcPhaseMapConverterCommand()
{
}


std::string gkg::GehcPhaseMapConverterCommand::getStaticName()
{

  try
  {

    return "GehcPhaseMapConverter";

  }
  GKG_CATCH( "std::string gkg::GehcPhaseMapConverterCommand::getStaticName()" );

}


void gkg::GehcPhaseMapConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "GE HealthCare int16_t to float "
                                  "phase map converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input int16_t GE HealthCare phase map",
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
  GKG_CATCH_FUNCTOR( "void gkg::GehcPhaseMapConverterCommand::parse()" );

}


void gkg::GehcPhaseMapConverterCommand::execute(
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

      std::cout << "reading GE HealthCare phase map '"
                << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< int16_t > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // converting from int16_t to float
    if ( verbose )
    {

      std::cout << "converting from int16_t to float : " << std::flush;

    }
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();
    int32_t sizeT = inputVolume.getSizeT();

    // Sanity check
    if ( sizeT != 8 )
    {

      throw std::runtime_error( "Error: input sizeT should be equal to 8 !" );

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

    gkg::Volume< float > phaseVolume( sizeX, sizeY, sizeZ, 2 );
    phaseVolume.getHeader().addAttribute( "resolutionX", resolutionX );
    phaseVolume.getHeader().addAttribute( "resolutionY", resolutionY );
    phaseVolume.getHeader().addAttribute( "resolutionZ", resolutionZ );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          std::complex< float > c1( ( float )inputVolume( x, y, z, 2 ),
                                    ( float )inputVolume( x, y, z, 3 ) );
          std::complex< float > c2( ( float )inputVolume( x, y, z, 6 ),
                                    ( float )inputVolume( x, y, z, 7 ) );

          phaseVolume( x, y, z, 0 ) =  std::arg< float >( c1 );
          phaseVolume( x, y, z, 1 ) =  std::arg< float >( c2 );


        }

      }

    }

/*
    float minimumValue = ( float )inputVolume( 0, 0, 0, 1 );
    float maximumValue = ( float )inputVolume( 0, 0, 0, 1 );

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          phaseVolume( x, y, z, 0 ) =  ( float )inputVolume( x, y, z, 1 );
          phaseVolume( x, y, z, 1 ) =  ( float )inputVolume( x, y, z, 5 );

          if ( phaseVolume( x, y, z, 0 ) < minimumValue )
          {

             minimumValue = phaseVolume( x, y, z, 0 );

          }
          else if ( phaseVolume( x, y, z, 0 ) > maximumValue )
          {

             maximumValue = phaseVolume( x, y, z, 0 );

          }
          if ( phaseVolume( x, y, z, 1 ) < minimumValue )
          {

             minimumValue = phaseVolume( x, y, z, 1 );

          }
          else if ( phaseVolume( x, y, z, 1 ) > maximumValue )
          {

             maximumValue = phaseVolume( x, y, z, 1 );

          }

        }

      }

    }

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          phaseVolume( x, y, z, 0 ) = 2 * M_PI * ( phaseVolume( x, y, z, 0 ) -
                                                   minimumValue ) /
                                                 ( maximumValue -
                                                   minimumValue );
          phaseVolume( x, y, z, 1 ) = 2 * M_PI * ( phaseVolume( x, y, z, 1 ) -
                                                   minimumValue ) /
                                                 ( maximumValue -
                                                   minimumValue );

        }

      }

    }
*/

    gkg::Volume< float > outputVolume( sizeX, sizeY, sizeZ );
    outputVolume.getHeader().addAttribute( "resolutionX", resolutionX );
    outputVolume.getHeader().addAttribute( "resolutionY", resolutionY );
    outputVolume.getHeader().addAttribute( "resolutionZ", resolutionZ );

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

           outputVolume( x, y, z ) = wrapPhase( phaseVolume( x, y, z, 1 ) -
                                                phaseVolume( x, y, z, 0 ) );

        }

      }

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
  GKG_CATCH( "void gkg::GehcPhaseMapConverterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool ascii, bool verbose )" );

}


RegisterCommandCreator( GehcPhaseMapConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
