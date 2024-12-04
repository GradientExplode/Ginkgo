#include <gkg-mri-reconstruction-plugin-functors/SmsRawDump/SmsRawDumpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-mri-reconstruction-io/SmsRawDiskFormat.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::SmsRawDumpCommand::SmsRawDumpCommand( int32_t argc,
                                           char* argv[],
                                           bool loadPlugin,
                                           bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SmsRawDumpCommand::SmsRawDumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SmsRawDumpCommand::SmsRawDumpCommand( const std::string& fileNameIn,
                                           const std::string& fileNameOut,
                                           const std::string& tagName )
                       : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             tagName );

  }
  GKG_CATCH( "gkg::SmsRawDumpCommand::SmsRawDumpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& tagName )" );

}


gkg::SmsRawDumpCommand::SmsRawDumpCommand( const gkg::Dictionary& parameters )
                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, tagName );
    
    execute( fileNameIn,
             fileNameOut,
             tagName );

  }
  GKG_CATCH( "gkg::SmsRawDumpCommand::SmsRawDumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SmsRawDumpCommand::~SmsRawDumpCommand()
{
}


std::string gkg::SmsRawDumpCommand::getStaticName()
{

  try
  {

    return "SmsRawDump";

  }
  GKG_CATCH( "std::string gkg::SmsRawDumpCommand::getStaticName()" );

}


void gkg::SmsRawDumpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";
    std::string tagName;

    gkg::Application application( _argc, _argv,
                                  "SMS raw data dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input SMS *.dat file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-tag",
                                 "SMS raw tag name to be searched",
                                 tagName,
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             tagName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SmsRawDumpCommand::parse()" );

}


void gkg::SmsRawDumpCommand::execute( const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& tagName )
{

  try
  {

    // reading header
    gkg::HeaderedObject object;

    gkg::SmsRawDiskFormat< float >::getInstance().readHeader(
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
          sy = object.getSyntaxSet().find( "__sms_raw__" );
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
  GKG_CATCH( "void gkg::SmsRawDumpCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& tagName )" );

}


RegisterCommandCreator(
    SmsRawDumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( tagName ) );
