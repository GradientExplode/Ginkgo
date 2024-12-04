#include <gkg-dmri-plugin-functors/DwiGeometrySimulator/DwiGeometrySimulatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/FileFinder.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand(
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
  GKG_CATCH( "gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand(
                                    const std::string& fileNameMembraneGeometry,
                                    const std::string& fileNameMembraneMesh,
                                    bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameMembraneGeometry,
             fileNameMembraneMesh,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand( "
             "const std::string& fileNameMembraneGeometry, "
             "const std::string& fileNameMembraneMesh, "
             "bool verbose )" );

}


gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMembraneGeometry );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMembraneMesh );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMembraneGeometry,
             fileNameMembraneMesh,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGeometrySimulatorCommand::DwiGeometrySimulatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiGeometrySimulatorCommand::~DwiGeometrySimulatorCommand()
{
}


std::string gkg::DwiGeometrySimulatorCommand::getStaticName()
{

  try
  {

    return "DwiGeometrySimulator";

  }
  GKG_CATCH( "std::string gkg::DwiGeometrySimulatorCommand::getStaticName()" );

}


void gkg::DwiGeometrySimulatorCommand::parse()
{

  try
  {

    std::string fileNameMembraneGeometry;
    std::string fileNameMembraneMesh;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Diffusion geometry simulator",
                                  _loadPlugin );

    application.addSingleOption( "-geometry",
                                 "Membrane geometry file name",
                                 fileNameMembraneGeometry );
    application.addSingleOption( "-membraneMeshFileName",
                                 "Output file name for the local membrane "
                                 "mesh renderings ",
                                 fileNameMembraneMesh );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    ////// launching parser
    application.initialize();

    execute( fileNameMembraneGeometry,
             fileNameMembraneMesh,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiGeometrySimulatorCommand::parse()" );

}


void gkg::DwiGeometrySimulatorCommand::execute(
                                    const std::string& fileNameMembraneGeometry,
                                    const std::string& fileNameMembraneMesh,
                                    bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::FileFinder fileFinder;
    if ( !fileFinder.locateFromPath( fileNameMembraneGeometry ) )
    {

      throw std::runtime_error( "Membrane geometry file name not found" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating membrane meshes
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating membrane meshes : " << std::flush;

    }


    std::map< std::string,
              gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > >
      meshes = gkg::MeshFactory::getInstance().getMeshes(
                                                     fileNameMembraneGeometry );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // fusionning the membrane meshes
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "collecting the membrane meshes : " << std::flush;

    }

    int32_t globalVertexCount = 0;
    int32_t globalPolygonCount = 0;

    std::map< std::string,
              gkg::RCPointer<
                gkg::MeshMap< int32_t, float, 3U > > >::const_iterator
      m = meshes.begin(),
      me = meshes.end();
    while ( m != me )
    {

      globalVertexCount += m->second->vertices.getSiteCount( 0 );
      globalPolygonCount += m->second->polygons.getPolygonCount( 0 );

      ++ m;

    }

    // preparing mesh accumulator
    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0, globalVertexCount, globalPolygonCount );

    m = meshes.begin();
    while ( m != me )
    {

      std::cout << m->first << " " << std::flush;
      meshAccumulator.add( *( m->second ) );

      ++ m;

    }

    // finaling filling the output mesh
    gkg::MeshMap< int32_t, float, 3U > meshMap;
    meshMap.add( meshAccumulator );

    if ( verbose )
    {

        std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the fusion mesh map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing the output mesh map : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameMembraneMesh,
                                      meshMap,
                                      false,
                                      "aimsmesh" );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiGeometrySimulatorCommand::execute( "
             "const std::string& fileNameMembraneGeometry, "
             "const std::string& fileNameMembraneMesh, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiGeometrySimulatorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameMembraneGeometry ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMembraneMesh ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
