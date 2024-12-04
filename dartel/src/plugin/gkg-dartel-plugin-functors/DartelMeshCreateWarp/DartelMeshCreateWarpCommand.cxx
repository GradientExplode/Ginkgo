#include <gkg-dartel-plugin-functors/DartelMeshCreateWarp/DartelMeshCreateWarpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dartel-mesh/DartelMeshTransformation.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
// class DartelMeshCreateWarpSubProcess
//


template < class L >
class DartelMeshCreateWarpSubProcess : public gkg::Process
{

  public:

    DartelMeshCreateWarpSubProcess( const std::string& theFileNameIn,
                                    const std::string& theFileNameFlowField,
                                    const std::string& theFileNameOut,
                                    const std::string& theFileNameTemplate,
                                    const int32_t& theK,
                                    const bool& theInverse,
                                    const bool& theVerbose );

    const std::string& fileNameIn;
    const std::string& fileNameFlowField;
    const std::string& fileNameOut;
    const std::string& fileNameTemplate;
    const int32_t& k;
    const bool& inverse;
    const bool& verbose;

  private:

    template < class S >
    static void operate( gkg::Process& process,
                         const std::string& fileNameInTemplate,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


template < class L >
DartelMeshCreateWarpSubProcess< L >::DartelMeshCreateWarpSubProcess(
                                        const std::string& theFileNameIn,
                                        const std::string& theFileNameFlowField,
                                        const std::string& theFileNameOut,
                                        const std::string& theFileNameTemplate,
                                        const int32_t& theK,
                                        const bool& theInverse,
                                        const bool& theVerbose )
                                   : gkg::Process(),
                                     fileNameIn( theFileNameIn ),
                                     fileNameFlowField( theFileNameFlowField ),
                                     fileNameOut( theFileNameOut ),
                                     fileNameTemplate( theFileNameTemplate ),
                                     k( theK ),
                                     inverse( theInverse ),
                                     verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DartelMeshCreateWarpSubProcess::operate< double > );

  }
  GKG_CATCH( "DartelMeshCreateWarpProcess::DartelMeshCreateWarpProcess( "
             "const std::string& theFileNameIn, "
             "const std::string& theFileNameFlowField, "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameTemplate, "
             "const int32_t& theK, const bool& theInverse, "
             "const bool& theVerbose )" );

}


template < class L > template < class S >
void DartelMeshCreateWarpSubProcess< L >::operate(
                                          gkg::Process& process,
                                          const std::string& fileNameInTemplate,
                                          const gkg::AnalyzedObject&,
                                          const std::string& )
{

  try
  {

    DartelMeshCreateWarpSubProcess< L >& dartelCreateWarpProcess = 
                 static_cast< DartelMeshCreateWarpSubProcess< L >& >( process );

    gkg::MeshMap< L, float, 3U > inMeshMap;
    gkg::Volume< S > inProxy;
    gkg::Volume< float > flowfield;
    gkg::Volume< S > templateProxy;
    gkg::MeshMap< L, float, 3U > outMeshMap;

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "reading input mesh map : " 
                << dartelCreateWarpProcess.fileNameIn;

    }
      gkg::Reader::getInstance().read( dartelCreateWarpProcess.fileNameIn,
                                     inMeshMap );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "reading input image header : " << fileNameInTemplate;

    }
    gkg::Reader::getInstance().readHeader( fileNameInTemplate, inProxy );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "reading flow field : " 
                << dartelCreateWarpProcess.fileNameFlowField;

    }
    gkg::Reader::getInstance().read( dartelCreateWarpProcess.fileNameFlowField,
                                     flowfield );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.inverse &&
         !dartelCreateWarpProcess.fileNameTemplate.empty() )
    {

      if ( dartelCreateWarpProcess.verbose )
      {

        std::cout << "reading template image header : " 
                  << dartelCreateWarpProcess.fileNameTemplate;

      }
      gkg::Reader::getInstance().readHeader(
                                       dartelCreateWarpProcess.fileNameTemplate,
                                       templateProxy );
      if ( dartelCreateWarpProcess.verbose )
      {

        std::cout << " - done" << std::endl;

      }

    }

    gkg::DartelMeshTransformation< L, S > dartel( dartelCreateWarpProcess.k );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "applying deformation" << std::flush;

    }
    if ( dartelCreateWarpProcess.inverse )
    {

      dartel.unwrap( inMeshMap, inProxy, flowfield, templateProxy, outMeshMap );

    }
    else
    {

      dartel.wrap( inMeshMap, inProxy, flowfield, outMeshMap );

    }
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "writing output mesh map : "
                << dartelCreateWarpProcess.fileNameOut << std::flush;

    }
    gkg::Writer::getInstance().write( dartelCreateWarpProcess.fileNameOut,
                                      outMeshMap );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > template < class S > "
             "void DartelMeshCreateWarpProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
// class DartelMeshCreateWarpProcess
//


class DartelMeshCreateWarpProcess : public gkg::Process
{

  public:

    DartelMeshCreateWarpProcess( const std::string& theFileNameInTemplate,
                                 const std::string& theFileNameFlowField,
                                 const std::string& theFileNameOut,
                                 const std::string& theFileNameTemplate,
                                 const int32_t& theK,
                                 const bool& theInverse,
                                 const bool& theVerbose );

    const std::string& fileNameInTemplate;
    const std::string& fileNameFlowField;
    const std::string& fileNameOut;
    const std::string& fileNameTemplate;
    const int32_t& k;
    const bool& inverse;
    const bool& verbose;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


DartelMeshCreateWarpProcess::DartelMeshCreateWarpProcess(
                                       const std::string& theFileNameInTemplate,
                                       const std::string& theFileNameFlowField,
                                       const std::string& theFileNameOut,
                                       const std::string& theFileNameTemplate,
                                       const int32_t& theK,
                                       const bool& theInverse,
                                       const bool& theVerbose )
                           : gkg::Process( "MeshMap" ),
                             fileNameInTemplate( theFileNameInTemplate ),
                             fileNameFlowField( theFileNameFlowField ),
                             fileNameOut( theFileNameOut ),
                             fileNameTemplate( theFileNameTemplate ),
                             k( theK ),
                             inverse( theInverse ),
                             verbose( theVerbose )
{

  try
  {

    registerProcess( "MeshMap", gkg::TypeOf< uint8_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< uint8_t > );
    registerProcess( "MeshMap", gkg::TypeOf< int8_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< int8_t > );
    registerProcess( "MeshMap", gkg::TypeOf< uint16_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< uint16_t > );
    registerProcess( "MeshMap", gkg::TypeOf< int16_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< int16_t > );
    registerProcess( "MeshMap", gkg::TypeOf< uint32_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< uint32_t > );
    registerProcess( "MeshMap", gkg::TypeOf< int32_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< int32_t > );
    registerProcess( "MeshMap", gkg::TypeOf< uint64_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< uint64_t > );
    registerProcess( "MeshMap", gkg::TypeOf< int64_t >::getName(),
                     &DartelMeshCreateWarpProcess::operate< int64_t > );

  }
  GKG_CATCH( "DartelMeshCreateWarpProcess::DartelMeshCreateWarpProcess( "
             "const std::string& theFileNameInTemplate, "
             "const std::string& theFileNameFlowField, "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameTemplate, "
             "const int32_t& theK, const bool& theInverse, "
             "const bool& theVerbose )" );

}


template < class L >
void DartelMeshCreateWarpProcess::operate( gkg::Process& process,
                                           const std::string& fileNameIn,
                                           const gkg::AnalyzedObject&,
                                           const std::string& )
{

  try
  {

    DartelMeshCreateWarpProcess& dartelCreateWarpProcess = 
                         static_cast< DartelMeshCreateWarpProcess& >( process );
    DartelMeshCreateWarpSubProcess< L > dartelSubProcess(
                                      fileNameIn,
                                      dartelCreateWarpProcess.fileNameFlowField,
                                      dartelCreateWarpProcess.fileNameOut,
                                      dartelCreateWarpProcess.fileNameTemplate,
                                      dartelCreateWarpProcess.k,
                                      dartelCreateWarpProcess.inverse,
                                      dartelCreateWarpProcess.verbose );

    dartelSubProcess.execute( dartelCreateWarpProcess.fileNameInTemplate );

  }
  GKG_CATCH( "template < class L > "
             "void DartelMeshCreateWarpProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
//   DartelMeshCreateWarpCommand
//


gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand(
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
  GKG_CATCH( "gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand(
                                          const std::string& fileNameIn,
                                          const std::string& fileNameInTemplate,
                                          const std::string& fileNameFlowField,
                                          const std::string& fileNameOut,
                                          const std::string& fileNameTemplate,
                                          int32_t k,
                                          bool inverse,
                                          bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameInTemplate, fileNameFlowField,
             fileNameOut, fileNameTemplate, k, inverse, verbose );

  }
  GKG_CATCH( "gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameInTemplate, "
             "const std::string& fileNameFlowField, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameTemplate, "
             "int32_t k, bool inverse, bool verbose )" );

}


gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInTemplate );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFlowField );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTemplate );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, k );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, inverse );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameInTemplate, fileNameFlowField,
             fileNameOut, fileNameTemplate, k, inverse, verbose );

  }
  GKG_CATCH( "gkg::DartelMeshCreateWarpCommand::DartelMeshCreateWarpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DartelMeshCreateWarpCommand::~DartelMeshCreateWarpCommand()
{
}


std::string gkg::DartelMeshCreateWarpCommand::getStaticName()
{

  try
  {

    return "DartelMeshCreateWarp";

  }
  GKG_CATCH( "std::string gkg::DartelMeshCreateWarpCommand::getStaticName()" );

}


void gkg::DartelMeshCreateWarpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameInTemplate;
    std::string fileNameFlowField;
    std::string fileNameOut;
    std::string fileNameTemplate = "";
    int32_t k = 6;
    bool inverse = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Dartel Create direct or inverse "
                                  "Warp of meshes",
                                  _loadPlugin );
    application.addSingleOption( "-m",
                                 "Mesh map to (un)warp",
                                 fileNameIn );
    application.addSingleOption( "-i",
                                 "Input image used to create the meshes "
                                 "for input dimension and transformation ",
                                 fileNameInTemplate );
    application.addSingleOption( "-u",
                                 "Flow field",
                                 fileNameFlowField );
    application.addSingleOption( "-k",
                                 "Log2 of the Euler time steps (default=6)",
                                 k,
                                 true );
    application.addSingleOption( "-inverse",
                                 "Create inverse warp",
                                 inverse,
                                 true );
    application.addSingleOption( "-o",
                                 "Warped or Unwarped meshes",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Template image for output dimension "
                                 "and transformation "
                                 "(only when 'inverse' option is set)",
                                 fileNameTemplate,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameInTemplate, fileNameFlowField,
             fileNameOut, fileNameTemplate, k, inverse, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelMeshCreateWarpCommand::parse()" );

}


void gkg::DartelMeshCreateWarpCommand::execute(
                                          const std::string& fileNameIn,
                                          const std::string& fileNameInTemplate,
                                          const std::string& fileNameFlowField,
                                          const std::string& fileNameOut,
                                          const std::string& fileNameTemplate,
                                          int32_t k,
                                          bool inverse,
                                          bool verbose )
{

  try
  {

    DartelMeshCreateWarpProcess dartelCreateWarpProcess( fileNameInTemplate,
                                                         fileNameFlowField,
                                                         fileNameOut,
                                                         fileNameTemplate,
                                                         k,
                                                         inverse,
                                                         verbose );
    dartelCreateWarpProcess.execute( fileNameIn );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelMeshCreateWarpCommand::execute( "
                     "const std::string& fileNameIn, "
                     "const std::string& fileNameInTemplate, "
                     "const std::string& fileNameFlowField, "
                     "const std::string& fileNameOut, "
                     "const std::string& fileNameTemplate, "
                     "int32_t k, bool inverse, bool verbose )" );

}


RegisterCommandCreator( DartelMeshCreateWarpCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameInTemplate ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameFlowField ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameTemplate ) +
    			DECLARE_INTEGER_PARAMETER_HELP( k ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( inverse ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
