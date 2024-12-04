#include <gkg-dmri-plugin-functors/DwiQSpaceSamplingRendering/DwiQSpaceSamplingRenderingCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiQSpaceSamplingRenderingCommand::DwiQSpaceSamplingRenderingCommand(
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
  GKG_CATCH( "gkg::DwiQSpaceSamplingRenderingCommand::"
             "DwiQSpaceSamplingRenderingCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQSpaceSamplingRenderingCommand::DwiQSpaceSamplingRenderingCommand(
                                                const std::string& fileNameDW,
                                                const std::string& fileNameMesh,
                                                float sphereRadius,
                                                int32_t sphereVertexCount,
                                                bool symmetrical,
                                                const std::string& meshFormat,
                                                bool ascii,
                                                bool verbose )
                                       : gkg::Command()
{

  try
  {

    execute( fileNameDW,
             fileNameMesh,
             sphereRadius,
             sphereVertexCount,
             symmetrical,
             meshFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceSamplingRenderingCommand::"
             "DwiQSpaceSamplingRenderingCommand( "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMesh, "
             "float sphereRadius, "
             "int32_t sphereVertexCount, "
             "bool symmetrical, "
             "const std::string& meshFormat, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiQSpaceSamplingRenderingCommand::DwiQSpaceSamplingRenderingCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMesh );
    DECLARE_FLOATING_PARAMETER( parameters, float, sphereRadius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sphereVertexCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, symmetrical );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameDW,
             fileNameMesh,
             sphereRadius,
             sphereVertexCount,
             symmetrical,
             meshFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceSamplingRenderingCommand::"
             "DwiQSpaceSamplingRenderingCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQSpaceSamplingRenderingCommand::~DwiQSpaceSamplingRenderingCommand()
{
}


std::string gkg::DwiQSpaceSamplingRenderingCommand::getStaticName()
{

  try
  {

    return "DwiQSpaceSamplingRendering";

  }
  GKG_CATCH( "std::string gkg::DwiQSpaceSamplingRenderingCommand::getStaticName()" );

}


void gkg::DwiQSpaceSamplingRenderingCommand::parse()
{

  try
  {

    std::string fileNameDW;
    std::string fileNameMesh;
    float sphereRadius = 1.0;
    int32_t sphereVertexCount = 30;
    bool symmetrical = false;
    std::string meshFormat = "aimsmesh";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI Q-space rendering tool",
                                  _loadPlugin );
    application.addSingleOption( "-dw",
                                 "Input DW-weighted volume file name",
                                 fileNameDW );
    application.addSingleOption( "-o",
                                 "Output Q-space sampling mesh map",
                                 fileNameMesh );
    application.addSingleOption( "-sphereRadius",
                                 "Radius of sphere for point rendering"
                                 "(default=2*min. res.)",
                                 sphereRadius,
                                 true );
    application.addSingleOption( "-sphereVertexCount",
                                 "Sphere vertex count (default=30)",
                                 sphereVertexCount,
                                 true );
    application.addSingleOption( "-symmetrical",
                                 "Also plot symmetrical Q-space points "
                                 "(default=false)",
                                 symmetrical,
                                 true );
    application.addSingleOption( "-meshFormat",
                                 "Ouput mesh format (default=aimsmesh)",
                                 meshFormat,
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

    execute( fileNameDW,
             fileNameMesh,
             sphereRadius,
             sphereVertexCount,
             symmetrical,
             meshFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQSpaceSamplingRenderingCommand::parse()" );

}


void gkg::DwiQSpaceSamplingRenderingCommand::execute(
                                                const std::string& fileNameDW,
                                                const std::string& fileNameMesh,
                                                float sphereRadius,
                                                int32_t sphereVertexCount,
                                                bool symmetrical,
                                                const std::string& meshFormat,
                                                bool ascii,
                                                bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }

    gkg::Volume< float > dw;
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( dw );
    dwVolumeReaderProcess.execute( fileNameDW );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // obtaining input Q-space sampling from DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "obtaining input Q-space sampling : " << std::flush;

    }
    gkg::RCPointer< gkg::QSpaceSampling > 
      qSpaceSampling( gkg::QSpaceSamplingFactory::getInstance().create( dw ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building Q-space sampling rendering
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building Q-space sampling rendering : " << std::flush;

    }
    std::list< gkg::MeshMap< int32_t, float, 3U > > meshes;
    qSpaceSampling->getMeshes( sphereRadius, sphereVertexCount, symmetrical,
                               meshes );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving result
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving ' " << fileNameMesh << "' : " << std::flush;

    }
    std::list< gkg::MeshMap< int32_t, float, 3U > >::iterator
      m = meshes.begin(),
      me = meshes.end();
    int32_t count = 1;
    while ( m != me )
    {

      gkg::Writer::getInstance().write( fileNameMesh + "_part" +
                                        gkg::StringConverter::toString( count ),
                                        *m,
                                        ascii,
                                        meshFormat );
      ++ m;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiQSpaceSamplingRenderingCommand::execute( "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMesh, "
             "float sphereRadius, "
             "int32_t sphereVertexCount, "
             "bool symmetrical, "
             "const std::string& meshFormat, "
             "bool ascii, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    DwiQSpaceSamplingRenderingCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMesh ) +
    DECLARE_FLOATING_PARAMETER_HELP( sphereRadius ) +
    DECLARE_INTEGER_PARAMETER_HELP( sphereVertexCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( symmetrical ) +
    DECLARE_STRING_PARAMETER_HELP( meshFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
