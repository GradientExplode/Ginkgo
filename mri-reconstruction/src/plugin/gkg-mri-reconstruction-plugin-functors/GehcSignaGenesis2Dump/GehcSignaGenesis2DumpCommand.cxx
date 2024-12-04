#include <gkg-mri-reconstruction-plugin-functors/GehcSignaGenesis2Dump/GehcSignaGenesis2DumpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-mri-reconstruction-io/GehcSignaGenesis2DiskFormat.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand(
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
  GKG_CATCH( "gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut )
                                  : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut );

  }
  GKG_CATCH( "gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut )" );

}


gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand(
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
  GKG_CATCH( "gkg::GehcSignaGenesis2DumpCommand::GehcSignaGenesis2DumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaGenesis2DumpCommand::~GehcSignaGenesis2DumpCommand()
{
}


std::string gkg::GehcSignaGenesis2DumpCommand::getStaticName()
{

  try
  {

    return "GehcSignaGenesis2Dump";

  }
  GKG_CATCH( "std::string gkg::GehcSignaGenesis2DumpCommand::getStaticName()" );

}


void gkg::GehcSignaGenesis2DumpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";

    gkg::Application application( _argc, _argv,
                                  "GEHC Signa 1.5T Genesis 2 dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Genesis 2  file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcSignaGenesis2DumpCommand::parse()" );

}


void gkg::GehcSignaGenesis2DumpCommand::execute(
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
    gkg::GehcSignaGenesis2DiskFormat< int16_t >::getInstance().readHeader(
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
  GKG_CATCH( "void gkg::GehcSignaGenesis2DumpCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut )" );

}


RegisterCommandCreator( 
    GehcSignaGenesis2DumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) );
