#include <gkg-processing-plugin-functors/MeshMapSurfaceComputer/MeshMapSurfaceComputerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/MeshSurfaceComputer_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <list>


//
// class MeshMapSurfaceComputerProcess
//


class MeshMapSurfaceComputerProcess : public gkg::Process
{

  public:

    MeshMapSurfaceComputerProcess(
                      const std::string& theFileNameOut,
                      const bool& theVerbose );

    const std::string& fileNameOut;
    const bool& verbose;

  private:

    template < class R >
    static void compute( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


MeshMapSurfaceComputerProcess::MeshMapSurfaceComputerProcess(
                                              const std::string& theFileNameOut,
                                              const bool& theVerbose )
                              : gkg::Process( "MeshMap" ),
                                fileNameOut( theFileNameOut ),
                                verbose( theVerbose )
{

  try
  {

    registerProcess( "MeshMap", gkg::TypeOf< int32_t >::getName(),
                     &MeshMapSurfaceComputerProcess::compute< int32_t > );

  }
  GKG_CATCH( "MeshMapSurfaceComputerProcess::"
             "MeshMapSurfaceComputerProcess( "
	     "const std::string& theFileNameOut, "
	     "const std::vector< std::string >& theFileNameTransform3ds, "
	     "const bool& theVerbose )" );

}


template < class R >
void 
MeshMapSurfaceComputerProcess::compute( gkg::Process& process,
                                        const std::string& fileNameIn,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    MeshMapSurfaceComputerProcess&
      meshMapTransform3dProcess =
        static_cast< MeshMapSurfaceComputerProcess& >( process );

    const std::string& 
      fileNameOut = meshMapTransform3dProcess.fileNameOut;
    const bool&
      verbose = meshMapTransform3dProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameIn << "' mesh map : "
                << std::flush;
     
    }
    gkg::MeshMap< R, float, 3U > meshMap;
    std::string format = gkg::Reader::getInstance().read( fileNameIn,
                                                          meshMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    //////////////////////////////////////////////////////////////////////////
    // computing surface areas in mm2
    //////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing surface areas in mm2 : " << std::flush;

    }
    
    std::map< R, float > surfaceAreas;
    gkg::MeshSurfaceComputer< R, float, 3U > meshSurfaceComputer;
    meshSurfaceComputer.compute( meshMap, surfaceAreas );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output surface area CSV file
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "writing output '" << fileNameOut << "' surface areas CSV : "
                << std::flush;
     
    }

    std::ofstream os( fileNameOut.c_str() );

    typename std::map< R, float >::const_iterator
      s = surfaceAreas.begin(),
      se = surfaceAreas.end();
    while ( s != se )
    {

      os << s->first << " " << s->second << std::endl;
      ++ s;

    }

    os.close();

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }

  }
  GKG_CATCH( "template < class R > "
             "void MeshMapSurfaceComputerProcess::compute( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// MeshMapSurfaceComputerCommand
//


gkg::MeshMapSurfaceComputerCommand::MeshMapSurfaceComputerCommand(
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
  GKG_CATCH( "gkg::MeshMapSurfaceComputerCommand::"
             "MeshMapSurfaceComputerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MeshMapSurfaceComputerCommand::MeshMapSurfaceComputerCommand(
                                             const std::string& fileNameMeshMap,
                                             const std::string& fileNameOut,
                                             bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameMeshMap, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::MeshMapSurfaceComputerCommand::"
             "MeshMapSurfaceComputerCommand( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::MeshMapSurfaceComputerCommand::MeshMapSurfaceComputerCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMeshMap, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::MeshMapSurfaceComputerCommand::"
             "MeshMapSurfaceComputerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MeshMapSurfaceComputerCommand::~MeshMapSurfaceComputerCommand()
{
}


std::string gkg::MeshMapSurfaceComputerCommand::getStaticName()
{

  try
  {

    return "MeshMapSurfaceComputer";

  }
  GKG_CATCH( "std::string "
             "gkg::MeshMapSurfaceComputerCommand::getStaticName()" );

}


void gkg::MeshMapSurfaceComputerCommand::parse()
{

  try
  {

    std::string fileNameMeshMap;
    std::string fileNameOut;
    std::vector< std::string > fileNameTransform3ds;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Bundle map 3D transformer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input mesh map",
                                 fileNameMeshMap );
    application.addSingleOption( "-o",
                                 "Output mesh map",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 

    application.initialize(); 			 

    execute( fileNameMeshMap, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MeshMapSurfaceComputerCommand::parse()" );

}


void gkg::MeshMapSurfaceComputerCommand::execute(
                                             const std::string& fileNameMeshMap,
                                             const std::string& fileNameOut,
                                             bool verbose )
{

  try
  {

    MeshMapSurfaceComputerProcess meshMapTransform3dProcess( fileNameOut,
                                                             verbose );
    meshMapTransform3dProcess.execute( fileNameMeshMap );

  }
  GKG_CATCH( "void gkg::MeshMapSurfaceComputerCommand::execute( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( MeshMapSurfaceComputerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameMeshMap ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
