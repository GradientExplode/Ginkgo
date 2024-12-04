#include <gkg-dmri-plugin-functors/DwiGeometricMean/DwiGeometricMeanCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>


gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand( 
                                              const std::string& fileNameInput,
                                              const std::string& fileNameOutput,
                                              bool ascii,
                                              const std::string& format,
                                              bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameInput,
             fileNameOutput,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand( "
             "const std::string& fileNameInput, "
             "const std::string& fileNameOutput, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInput );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInput,
             fileNameOutput,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGeometricMeanCommand::DwiGeometricMeanCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiGeometricMeanCommand::~DwiGeometricMeanCommand()
{
}


std::string gkg::DwiGeometricMeanCommand::getStaticName()
{

  try
  {

    return "DwiGeometricMean";

  }
  GKG_CATCH( "std::string gkg::DwiGeometricMeanCommand::getStaticName()" );

}


void gkg::DwiGeometricMeanCommand::parse()
{

  try
  {

    std::string fileNameInput;
    std::string fileNameOutput;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Geometric mean of all DW volumes",
                                  _loadPlugin );
    application.addSingleOption( "-dw",
                                 "Input DW 4D volume",
                                 fileNameInput );
    application.addSingleOption( "-o",
                                 "Output DW 3D volume",
                                 fileNameOutput );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
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

    execute( fileNameInput,
             fileNameOutput,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiGeometricMeanCommand::parse()" );

}


void gkg::DwiGeometricMeanCommand::execute( const std::string& fileNameInput,
                                            const std::string& fileNameOutput,
                                            bool ascii,
                                            const std::string& format,
                                            bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading input DW 4D volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameInput << "' : " << std::flush;

    }
    gkg::Volume< float > inputVolume;
    gkg::TypedVolumeReaderProcess< float >
     inputVolumeReaderProcess( inputVolume );
    inputVolumeReaderProcess.execute( fileNameInput );

    const int32_t inputSizeX = inputVolume.getSizeX();
    const int32_t inputSizeY = inputVolume.getSizeY();
    const int32_t inputSizeZ = inputVolume.getSizeZ();
    const int32_t inputSizeT = inputVolume.getSizeT();
    gkg::Vector3d< double > inputResolution;
    inputVolume.getResolution( inputResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating output DW 4D volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > outputVolume( inputSizeX, inputSizeY, inputSizeZ );
    outputVolume.setResolution( inputResolution );


    ////////////////////////////////////////////////////////////////////////////
    // filling output DW 4D volume
    ////////////////////////////////////////////////////////////////////////////
    double n = 1.0 / inputSizeT;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;

    for ( z = 0; z < inputSizeZ; z ++ )
    {

      for ( y = 0; y < inputSizeY; y ++ )
      {

        for ( x = 0; x < inputSizeX; x ++ )
        {

          double v = 1.0;
          for ( t = 0; t < inputSizeT; t ++ )
          {

            v *= std::pow( inputVolume( x, y, z, t ), n );

          }
          outputVolume( x, y, z ) = ( float )v;

        }

      }

    }


    if ( verbose )
    {

      std::cout << "writing output volume \'" << fileNameOutput << "\' : "
                << std::flush;

    }

    // Write output volume
    gkg::Writer::getInstance().write( fileNameOutput,
                                      outputVolume,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiGeometricMeanCommand::execute( "
             "const std::string& fileNameInput, "
             "const std::string& fileNameOutput, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiGeometricMeanCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameInput ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
 
