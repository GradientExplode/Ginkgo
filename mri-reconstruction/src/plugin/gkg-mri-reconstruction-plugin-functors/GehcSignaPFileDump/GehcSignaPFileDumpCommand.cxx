#include <gkg-mri-reconstruction-plugin-functors/GehcSignaPFileDump/GehcSignaPFileDumpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-mri-reconstruction-io/GehcSignaPFileDiskFormat.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand( int32_t argc,
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
  GKG_CATCH( "gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool printOption,
                                                 const std::string& tagName )
                               : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             printOption,
             tagName );

  }
  GKG_CATCH( "gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool printOption, "
             "const std::string& tagName )" );

}


gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, printOption );
    DECLARE_STRING_PARAMETER( parameters, std::string, tagName );
    
    execute( fileNameIn,
             fileNameOut,
             printOption,
             tagName );

  }
  GKG_CATCH( "gkg::GehcSignaPFileDumpCommand::GehcSignaPFileDumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaPFileDumpCommand::~GehcSignaPFileDumpCommand()
{
}


std::string gkg::GehcSignaPFileDumpCommand::getStaticName()
{

  try
  {

    return "GehcSignaPFileDump";

  }
  GKG_CATCH( "std::string gkg::GehcSignaPFileDumpCommand::getStaticName()" );

}


void gkg::GehcSignaPFileDumpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";
    bool printOption = false;
    std::string tagName;

    gkg::Application application( _argc, _argv,
                                  "GEHC Signa 1.5T P-file dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input P-data file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-p",
                                 "Print header to standard output stream if "
                                 "output header name is not given",
                                 printOption,
                                 true );
    application.addSingleOption( "-tag",
                                 "p-file tag name to be searched",
                                 tagName,
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             printOption,
             tagName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcSignaPFileDumpCommand::parse()" );

}


void gkg::GehcSignaPFileDumpCommand::execute( const std::string& fileNameIn,
                                              const std::string& fileNameOut,
                                              bool printOption,
                                              const std::string& tagName )
{

  try
  {

    // reading header
    gkg::HeaderedObject object;

    if ( fileNameOut.empty() && printOption )
    {

      gkg::GehcSignaPFileDiskFormat< float >::getInstance().setLogStream(
                                                                std::cout );

    }
    gkg::GehcSignaPFileDiskFormat< float >::getInstance().readHeader(
                                                   fileNameIn, object );

    if ( tagName.empty() )
    {

      // opening output file if provided
      std::ofstream* pOfstream = 0;
      if ( !fileNameOut.empty() )
      {

        pOfstream = new std::ofstream( fileNameOut.c_str() );

      }

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
    else
    {

      if ( object.getHeader().hasAttribute( tagName ) )
      {

        std::string typeName;
        gkg::SyntaxSet::const_iterator
          sy = object.getSyntaxSet().find( "__gehc_signa_pfile__" );
        if ( sy != object.getSyntaxSet().end() )
        {

          const gkg::Syntax& syntax = sy->second;
          gkg::Syntax::const_iterator
            se = syntax.find( tagName );

          if ( se != syntax.end() )
          {

            typeName = se->second.type;

          }

        }

        if ( typeName == "std_string" )
        {

          std::string value;
          object.getHeader().getAttribute( tagName, value );
          std::cout << value << std::endl;

        }
        else if ( typeName == "short" || typeName == "int16_t" )
        {

          int16_t value;
          object.getHeader().getAttribute( tagName, value );
          std::cout << value << std::endl;

        }
        else if ( typeName == "long" || typeName == "int32_t" )
        {

          int32_t value;
          object.getHeader().getAttribute( tagName, value );
          std::cout << value << std::endl;

        }
        else if ( typeName == "unsigned_long" || typeName == "uint32_t" )
        {

          uint32_t value;
          object.getHeader().getAttribute( tagName, value );
          std::cout << value << std::endl;

        }
        else if ( typeName == "float" )
        {

          float value;
          object.getHeader().getAttribute( tagName, value );
          std::cout << value << std::endl;

        }
        else
        {

          throw std::runtime_error( std::string( "tag " ) + tagName +
                                    " has a type not managed by dumper" );

        }

      }
      else
      {

        throw std::runtime_error( std::string( "tag " ) + tagName +
                                  " not found" );

      }

    }

  }
  GKG_CATCH( "void gkg::GehcSignaPFileDumpCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool printOption, "
             "const std::string& tagName )" );

}


RegisterCommandCreator( 
    GehcSignaPFileDumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( printOption ) +
    DECLARE_STRING_PARAMETER_HELP( tagName ) );
