#include <gkg-mri-reconstruction-plugin-functors/GehcSignaGenesisDump/GehcSignaGenesisDumpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-mri-reconstruction-io/GehcSignaGenesisDiskFormat.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                 : gkg::Command( argc, argv, loadPlugin,
                                                 removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut );

  }
  GKG_CATCH( "gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut  )" );

}


gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    
    execute( fileNameIn,
             fileNameOut );

  }
  GKG_CATCH( "gkg::GehcSignaGenesisDumpCommand::GehcSignaGenesisDumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaGenesisDumpCommand::~GehcSignaGenesisDumpCommand()
{
}


std::string gkg::GehcSignaGenesisDumpCommand::getStaticName()
{

  try
  {

    return "GehcSignaGenesisDump";

  }
  GKG_CATCH( "std::string gkg::GehcSignaGenesisDumpCommand::getStaticName()" );

}


void gkg::GehcSignaGenesisDumpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";

    gkg::Application application( _argc, _argv,
                                  "GEHC Signa 1.5T Genesis dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Genesis file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcSignaGenesisDumpCommand::parse()" );

}


void gkg::GehcSignaGenesisDumpCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut )
{

  try
  {

    // opening output file if provided
    std::ofstream* pOfstream = 0;
    if ( !fileNameOut.empty() )
    {

      pOfstream = new std::ofstream( fileNameOut.c_str() );

    }

    gkg::HeaderedObject object;
    gkg::GehcSignaGenesisDiskFormat< int16_t >::getInstance().readHeader(
                                                           fileNameIn, object );

    // writing header
    gkg::BaseObjectWriter baseObjectWriter;
    if ( pOfstream )
    {

      baseObjectWriter.write( *pOfstream,
                              object.getHeader() );

    }
    else
    {

      baseObjectWriter.write( std::cout,
                              object.getHeader() );

    }


    // closing and removing output file stream
    if ( !fileNameOut.empty() )
    {

      pOfstream->close();
      delete pOfstream;

    }

  }
  GKG_CATCH( "void gkg::GehcSignaGenesisDumpCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut  )" );

}


RegisterCommandCreator( 
    GehcSignaGenesisDumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) );
