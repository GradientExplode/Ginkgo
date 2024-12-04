#include <gkg-processing-plugin-functors/Transform3dComposer/Transform3dComposerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::Transform3dComposerCommand::Transform3dComposerCommand( int32_t argc,
                                                             char* argv[],
                                                             bool loadPlugin,
                                                             bool removeFirst )
                                : gkg::Command( argc,
                                                argv,
                                                loadPlugin,
                                                removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Transform3dComposerCommand::Transform3dComposerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Transform3dComposerCommand::Transform3dComposerCommand(
                                  const std::vector< std::string >& fileNameIns,
                                  const std::string& fileNameOut,
                                  bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIns, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Transform3dComposerCommand::Transform3dComposerCommand( "
             "const std::vector< std::string >& fileNameIns, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::Transform3dComposerCommand::Transform3dComposerCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameIns );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIns, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Transform3dComposerCommand::Transform3dComposerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Transform3dComposerCommand::~Transform3dComposerCommand()
{
}


std::string gkg::Transform3dComposerCommand::getStaticName()
{

  try
  {

    return "Transform3dComposer";

  }
  GKG_CATCH( "std::string gkg::Transform3dComposerCommand::getStaticName()" );

}


void gkg::Transform3dComposerCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameIns;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D transform composer (restricted to "
                                  "homogeneous 3D transforms)",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input file names e.g. T1.trm T2.trm ... "
                                 "TN.trm leading to the 3D transform "
                                 "I = TN ... x T2 x T1 such that p'= I x p",
                                 fileNameIns,
                                 2 );
    application.addSingleOption( "-o",
                                 "Output transform file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    execute(  fileNameIns, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Transform3dComposerCommand::parse()" );

}


void gkg::Transform3dComposerCommand::execute(
                                  const std::vector< std::string >& fileNameIns,
                                  const std::string& fileNameOut,
                                  bool verbose )
{

  try
  {
    //
    // creating output 3D composite transform
    //
    if ( verbose )
    {

      std::cout << "creating composite 3D transform : " << std::flush;

    }
    gkg::CompositeTransform3d< float > compositeTransform3d;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // reading and composing 3D transform(s)
    //
    int32_t count = ( int32_t )fileNameIns.size();
    int32_t t = 0;
    for ( t = 0; t < count; t++ )
    {

      if ( verbose )
      {

        std::cout << "reading and composing '" << fileNameIns[ t ] 
                  << "' 3D transform : "
                  << std::flush;

      }

      gkg::HomogeneousTransform3d< float > transform3d;
      std::ifstream is( fileNameIns[ t ].c_str() );
      if ( !is )
      {

        throw std::runtime_error( std::string( "could not open '" ) +
                                  fileNameIns[ t ] + "' file" );

      }
      transform3d.readTrm( is );
      is.close();

      compositeTransform3d.compose( transform3d );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

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
    compositeTransform3d.getComposition().writeTrm( os );
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::Transform3dComposerCommand::execute( "
             "const std::vector< std::string >& fileNameIns, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator(
                       Transform3dComposerCommand,
    		       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameIns ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
