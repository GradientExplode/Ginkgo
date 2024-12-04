#include <gkg-processing-plugin-functors/MeshMapToObjAndMtlConverter/MeshMapToObjAndMtlConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <list>


//
//   MeshMapToObjAndMtlConverterProcess
//


class MeshMapToObjAndMtlConverterProcess : public gkg::Process
{

  public:

    MeshMapToObjAndMtlConverterProcess( const std::string& theFileNameObj,
	                                const std::string& theFileNameMtl,
                                        const bool& theVerbose );

    const std::string& fileNameObj;
    const std::string& fileNameMtl;
    const bool& verbose;

  private:

    template < class L >
    static void convert( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


MeshMapToObjAndMtlConverterProcess::MeshMapToObjAndMtlConverterProcess(
                                          const std::string& theFileNameObj,
	                                  const std::string& theFileNameMtl,
	                                  const bool& theVerbose )
                                     : gkg::Process( "MeshMap" ),
                                       fileNameObj( theFileNameObj ),
                                       fileNameMtl( theFileNameMtl ),
                                       verbose( theVerbose )
{

  try
  {

    registerProcess( "MeshMap", gkg::TypeOf< int32_t >::getName(),
                    &MeshMapToObjAndMtlConverterProcess::convert< int32_t > );

  }
  GKG_CATCH( "MeshMapToObjAndMtlConverterProcess::"
             "MeshMapToObjAndMtlConverterProcess( "
	     "const std::string& theFileNameObj, "
	     "const std::string& theFileNameMtl, "
             "const float& theStep, "
	     "const float& theRadius, "
	     "const int32_t& theVertexCountPerPlane, "
	     "const bool& theVerbose )" );

}


template < class L >
void 
MeshMapToObjAndMtlConverterProcess::convert( gkg::Process& process,
 					     const std::string& fileNameIn,
 					     const gkg::AnalyzedObject&,
 					     const std::string& )
{

  try
  {

    MeshMapToObjAndMtlConverterProcess&
      meshMapToObjAndMtlConverterProcess =
        static_cast< MeshMapToObjAndMtlConverterProcess& >( process );

    const std::string& 
      fileNameObj = meshMapToObjAndMtlConverterProcess.fileNameObj;
    const bool&
      verbose = meshMapToObjAndMtlConverterProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameIn << "' mesh map : "
                << std::flush;
     
    }
    gkg::MeshMap< L, float, 3U > meshMap;
    gkg::Reader::getInstance().read( fileNameIn, meshMap );
    int32_t meshCount = meshMap.vertices.getRankCount();

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }

    //////////////////////////////////////////////////////////////////////////
    // opening output stream
    //////////////////////////////////////////////////////////////////////////

    std::ofstream os( fileNameObj.c_str() );
    if ( os.fail() )
    {

      throw std::runtime_error( "could not open .obj file" );

    }


    //////////////////////////////////////////////////////////////////////////
    // converting meshes
    //////////////////////////////////////////////////////////////////////////


    int32_t m = 0;
    for ( m = 0; m < meshCount; m++ )
    {

      L rank = meshMap.vertices.getRank( m );

      if ( verbose )
      {

        std::cout << "processing mesh '" << rank << "' : " << std::flush;

      }

      std::string fileNameCurrentObj = fileNameObj;
      if ( meshCount > 1 )
      {

        fileNameCurrentObj = fileNameObj + '-' +
                             gkg::StringConverter::toString( rank ) +
                             ".obj";

      }
      std::ofstream os( fileNameCurrentObj.c_str() );
      if ( os.fail() )
      {

        throw std::runtime_error( "could not open .obj file" );

      }

      const std::list< gkg::Vector3d< float > >&
        sites = meshMap.vertices.getSites( rank );

      std::list< gkg::Vector3d< float > >::const_iterator
        s = sites.begin(),
          se = sites.end();
      while ( s != se )
      {

        os << "v " << s->x << " " << s->y << " " << s->z << std::endl;
        ++ s;

      }

      os << std::endl;

      const std::list< gkg::Polygon< 3U > >& 
        polygons = meshMap.polygons.getPolygons( m );

      std::list< gkg::Polygon< 3U > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        os << "f " << p->indices[ 0 ] + 1 << " "
                   << p->indices[ 1 ] + 1 << " "
                   << p->indices[ 2 ] + 1 << std::endl;
                     
        ++ p;

      }

      os << std::endl;
      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void MeshMapToObjAndMtlConverterProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   MeshMapToObjAndMtlConverterCommand
//


gkg::MeshMapToObjAndMtlConverterCommand::MeshMapToObjAndMtlConverterCommand(
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
  GKG_CATCH( "gkg::MeshMapToObjAndMtlConverterCommand::MeshMapToObjAndMtlConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MeshMapToObjAndMtlConverterCommand::MeshMapToObjAndMtlConverterCommand(
                                             const std::string& fileNameMeshMap,
                                             const std::string& fileNameObj,
                                             const std::string& fileNameMtl,
                                             bool verbose )
                                       : gkg::Command()
{

  try
  {

    execute( fileNameMeshMap, fileNameObj, fileNameMtl, verbose );

  }
  GKG_CATCH( "gkg::MeshMapToObjAndMtlConverterCommand::MeshMapToObjAndMtlConverterCommand( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameObj, const std::string& fileNameMtl, "
             "bool verbose )" );

}


gkg::MeshMapToObjAndMtlConverterCommand::MeshMapToObjAndMtlConverterCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameObj );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMtl );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMeshMap, fileNameObj, fileNameMtl, verbose );

  }
  GKG_CATCH( "gkg::MeshMapToObjAndMtlConverterCommand::"
             "MeshMapToObjAndMtlConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MeshMapToObjAndMtlConverterCommand::~MeshMapToObjAndMtlConverterCommand()
{
}


std::string gkg::MeshMapToObjAndMtlConverterCommand::getStaticName()
{

  try
  {

    return "MeshMapToObjAndMtlConverter";

  }
  GKG_CATCH( "std::string gkg::MeshMapToObjAndMtlConverterCommand::getStaticName()" );

}


void gkg::MeshMapToObjAndMtlConverterCommand::parse()
{

  try
  {

    std::string fileNameMeshMap;
    std::string fileNameObj;
    std::string fileNameMtl;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Converting mesh map to .obj/.mtl",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input mesh map",
                                 fileNameMeshMap );
    application.addSingleOption( "-o",
                                 "Output Wavefront .obj file name",
                                 fileNameObj );
    application.addSingleOption( "-om",
                                 "Output materials .mtl file name",
                                 fileNameMtl );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 

    application.initialize(); 			 

    execute( fileNameMeshMap, fileNameObj, fileNameMtl, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MeshMapToObjAndMtlConverterCommand::parse()" );

}


void gkg::MeshMapToObjAndMtlConverterCommand::execute(
                  	                     const std::string& fileNameMeshMap,
                                             const std::string& fileNameObj,
                                             const std::string& fileNameMtl,
                                             bool verbose )
{

  try
  {

    MeshMapToObjAndMtlConverterProcess meshMapToObjAndMtlConverterProcess(
                                          			    fileNameObj,
                                          			    fileNameMtl,
                                          			    verbose );
    meshMapToObjAndMtlConverterProcess.execute( fileNameMeshMap );

  }
  GKG_CATCH( "void gkg::MeshMapToObjAndMtlConverterCommand::execute( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameObj, const std::string& fileNameMtl, "
             "bool verbose )" );

}


RegisterCommandCreator( MeshMapToObjAndMtlConverterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameMeshMap ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameObj ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMtl ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
