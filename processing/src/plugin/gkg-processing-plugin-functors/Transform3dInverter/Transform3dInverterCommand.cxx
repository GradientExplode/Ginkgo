#include <gkg-processing-plugin-functors/Transform3dInverter/Transform3dInverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::Transform3dInverterCommand::Transform3dInverterCommand( int32_t argc,
                                                             char* argv[],
                                                             bool loadPlugin,
                                                             bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Transform3dInverterCommand::Transform3dInverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Transform3dInverterCommand::Transform3dInverterCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Transform3dInverterCommand::Transform3dInverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::Transform3dInverterCommand::Transform3dInverterCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Transform3dInverterCommand::Transform3dInverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Transform3dInverterCommand::~Transform3dInverterCommand()
{
}


std::string gkg::Transform3dInverterCommand::getStaticName()
{

  try
  {

    return "Transform3dInverter";

  }
  GKG_CATCH( "std::string gkg::Transform3dInverterCommand::getStaticName()" );

}


void gkg::Transform3dInverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D transform inverter (restricted to "
                                  "homogeneous 3D transforms)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input 3D transform file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output 3D transform file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Transform3dInverterCommand::parse()" );

}


void gkg::Transform3dInverterCommand::execute( const std::string& fileNameIn,
                                               const std::string& fileNameOut,
                                               bool verbose )
{

  try
  {

    //
    // reading and composing 3D transform(s)
    //
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn 
                << "' 3D transform : "
                << std::flush;

    }

    gkg::HomogeneousTransform3d< float > transform3d;
    std::ifstream is( fileNameIn.c_str() );
    if ( !is )
    {

      throw std::runtime_error( std::string( "could not open '" ) +
                                fileNameIn + "' file" );

    }
    transform3d.readTrm( is );
    is.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    
    //
    // writing composition
    //

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' composition : "
                << std::flush;

    }

    std::ofstream os( fileNameOut.c_str() );
    if ( !os )
    {

      throw std::runtime_error( std::string( "could not open '" ) +
                                fileNameOut + "' file" );

    }
    transform3d.getInverseHomogeneousTransform3d().writeTrm( os );
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::Transform3dInverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( Transform3dInverterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
