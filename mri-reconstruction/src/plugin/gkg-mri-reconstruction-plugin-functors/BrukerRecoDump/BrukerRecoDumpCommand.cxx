#include <gkg-mri-reconstruction-plugin-functors/BrukerRecoDump/BrukerRecoDumpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-mri-reconstruction-io/BrukerRecoDiskFormat.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut )
                           : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut  );

  }
  GKG_CATCH( "gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut )" );

}


gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    
    execute( fileNameIn,
             fileNameOut  );

  }
  GKG_CATCH( "gkg::BrukerRecoDumpCommand::BrukerRecoDumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerRecoDumpCommand::~BrukerRecoDumpCommand()
{
}


std::string gkg::BrukerRecoDumpCommand::getStaticName()
{

  try
  {

    return "BrukerRecoDump";

  }
  GKG_CATCH( "std::string gkg::BrukerRecoDumpCommand::getStaticName()" );

}


void gkg::BrukerRecoDumpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";

    gkg::Application application( _argc, _argv,
                                  "Bruker Reco file dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Bruker Reco file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerRecoDumpCommand::parse()" );

}


void gkg::BrukerRecoDumpCommand::execute( const std::string& fileNameIn,
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

    // reading header
    gkg::HeaderedObject object;
    gkg::BrukerRecoDiskFormat< int16_t >::getInstance().readHeader(
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
  GKG_CATCH( "void gkg::BrukerRecoDumpCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut )" );

}


RegisterCommandCreator(
    BrukerRecoDumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn );
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) );
 
