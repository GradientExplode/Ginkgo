#include <gkg-processing-plugin-functors/AntipodallySymmetricUniformOrientationSet/AntipodallySymmetricUniformOrientationSetCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>


gkg::AntipodallySymmetricUniformOrientationSetCommand::
                               AntipodallySymmetricUniformOrientationSetCommand(
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
  GKG_CATCH( "gkg::AntipodallySymmetricUniformOrientationSetCommand::"
             "AntipodallySymmetricUniformOrientationSetCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AntipodallySymmetricUniformOrientationSetCommand::
                               AntipodallySymmetricUniformOrientationSetCommand(
                                  	       int32_t orientationCount,
                                  	       std::string& fileNameOut,
                                  	       bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( orientationCount, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::AntipodallySymmetricUniformOrientationSetCommand::AntipodallySymmetricUniformOrientationSetCommand( "
             "int32_t orientationCount, std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::AntipodallySymmetricUniformOrientationSetCommand::
                               AntipodallySymmetricUniformOrientationSetCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_INTEGER_PARAMETER( parameters, int32_t, orientationCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( orientationCount, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::AntipodallySymmetricUniformOrientationSetCommand::"
             "AntipodallySymmetricUniformOrientationSetCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AntipodallySymmetricUniformOrientationSetCommand::
                             ~AntipodallySymmetricUniformOrientationSetCommand()
{
}


std::string 
gkg::AntipodallySymmetricUniformOrientationSetCommand::getStaticName()
{

  try
  {

    return "AntipodallySymmetricUniformOrientationSet";

  }
  GKG_CATCH( "std::string "
             "gkg::AntipodallySymmetricUniformOrientationSetCommand::"
             "getStaticName()" );

}


void gkg::AntipodallySymmetricUniformOrientationSetCommand::parse()
{

  try
  {

    int32_t orientationCount = 0;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
             			  "Antipodally symmetric uniform distribution"
             			  "orientation set generator",
                                  _loadPlugin );

    application.addSingleOption( "-oc",
                                 "Orientation count",
                                 orientationCount,
                                 true );
    application.addSingleOption( "-o",
                                 "Output filename",
                                 fileNameOut );                             
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( orientationCount, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::AntipodallySymmetricUniformOrientationSetCommand::"
                     "parse()" );

}


void gkg::AntipodallySymmetricUniformOrientationSetCommand::execute(
                                  	       int32_t orientationCount,
                                  	       std::string& fileNameOut,
                                  	       bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "generating antipodally symmetric "
                << orientationCount << " orientation set : "
                << std::flush;

    }

    gkg::AntipodallySymmetricUniformOrientationSet
      orientationSet( orientationCount );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "saving orientation set : " << std::flush;

    }

    if ( fileNameOut.substr( fileNameOut.length() - 4U, 4U ) != ".txt" )
    {

      fileNameOut = fileNameOut + ".txt";

    }

    std::ofstream os( fileNameOut.c_str() );
    os << orientationSet.getCount() << std::endl;


    gkg::OrientationSet::const_iterator 
      o = orientationSet.begin(),
      oe = orientationSet.end();
    while ( o != oe )
    {

      os << o->x << " " << o->y << " " << o->z << std::endl;
      ++ o;

    }
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::AntipodallySymmetricUniformOrientationSetCommand::"
             "execute( int32_t orientationCount, std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( AntipodallySymmetricUniformOrientationSetCommand,
    			DECLARE_INTEGER_PARAMETER_HELP( orientationCount ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
