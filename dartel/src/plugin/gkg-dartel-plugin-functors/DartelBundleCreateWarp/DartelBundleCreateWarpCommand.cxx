#include <gkg-dartel-plugin-functors/DartelBundleCreateWarp/DartelBundleCreateWarpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dartel-bundle/DartelBundleTransformation.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


FORCE_HARD_LINK_WITH_CONNECTOMIST


//
// class DartelBundleCreateWarpSubProcess
//


template < class L >
class DartelBundleCreateWarpSubProcess : public gkg::Process
{

  public:

    DartelBundleCreateWarpSubProcess( const std::string& theFileNameIn,
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
DartelBundleCreateWarpSubProcess< L >::DartelBundleCreateWarpSubProcess(
                                       const std::string& theFileNameIn,
                                       const std::string& theFileNameFlowField,
                                       const std::string& theFileNameOut,
                                       const std::string& theFileNameTemplate,
                                       const int32_t& theK,
                                       const bool& theInverse,
                                       const bool& theVerbose )
                                     : gkg::Process(),
                                       fileNameIn( theFileNameIn ),
                                       fileNameFlowField(
                                                         theFileNameFlowField ),
                                       fileNameOut( theFileNameOut ),
                                       fileNameTemplate( theFileNameTemplate ),
                                       k( theK ),
                                       inverse( theInverse ),
                                       verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DartelBundleCreateWarpSubProcess::operate< double > );

  }
  GKG_CATCH( "DartelBundleCreateWarpProcess::DartelBundleCreateWarpProcess( "
             "const std::string& theFileNameIn, "
             "const std::string& theFileNameFlowField, "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameTemplate, "
             "const int32_t& theK, const bool& theInverse, "
             "const bool& theVerbose )" );

}


template < class L > template < class S >
void DartelBundleCreateWarpSubProcess< L >::operate(
                                          gkg::Process& process,
                                          const std::string& fileNameInTemplate,
                                          const gkg::AnalyzedObject&,
                                          const std::string& )
{

  try
  {

    DartelBundleCreateWarpSubProcess< L >& dartelCreateWarpProcess = 
               static_cast< DartelBundleCreateWarpSubProcess< L >& >( process );

    gkg::BundleMap< L > inBundleMap;
    gkg::Volume< S > inProxy;
    gkg::Volume< float > flowfield;
    gkg::Volume< S > templateProxy;
    gkg::BundleMap< L > outBundleMap;

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "reading input bundle map : " 
                << dartelCreateWarpProcess.fileNameIn;

    }
    gkg::Reader::getInstance().read( dartelCreateWarpProcess.fileNameIn,
                                     inBundleMap );
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

    gkg::DartelBundleTransformation< L, S > dartel( dartelCreateWarpProcess.k );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "applying deformation" << std::flush;

    }
    if ( dartelCreateWarpProcess.inverse )
    {

      dartel.unwrap( inBundleMap, inProxy, flowfield,
                     templateProxy, outBundleMap );

    }
    else
    {

      dartel.wrap( inBundleMap, inProxy, flowfield, outBundleMap );

    }
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "writing output bundle map : "
                << dartelCreateWarpProcess.fileNameOut << std::flush;

    }
    gkg::Writer::getInstance().write( dartelCreateWarpProcess.fileNameOut,
                                      outBundleMap );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > template < class S > "
             "void DartelBundleCreateWarpProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
// class DartelBundleCreateWarpProcess
//


class DartelBundleCreateWarpProcess : public gkg::Process
{

  public:

    DartelBundleCreateWarpProcess( const std::string& theFileNameInTemplate,
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


DartelBundleCreateWarpProcess::DartelBundleCreateWarpProcess(
                                       const std::string& theFileNameInTemplate,
                                       const std::string& theFileNameFlowField,
                                       const std::string& theFileNameOut,
                                       const std::string& theFileNameTemplate,
                                       const int32_t& theK,
                                       const bool& theInverse,
                                       const bool& theVerbose )
                             : gkg::Process( "BundleMap" ),
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

    registerProcess( "BundleMap", gkg::TypeOf< uint8_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< uint8_t > );
    registerProcess( "BundleMap", gkg::TypeOf< int8_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< int8_t > );
    registerProcess( "BundleMap", gkg::TypeOf< uint16_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< uint16_t > );
    registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< int16_t > );
    registerProcess( "BundleMap", gkg::TypeOf< uint32_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< uint32_t > );
    registerProcess( "BundleMap", gkg::TypeOf< int32_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< int32_t > );
    registerProcess( "BundleMap", gkg::TypeOf< uint64_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< uint64_t > );
    registerProcess( "BundleMap", gkg::TypeOf< int64_t >::getName(),
                     &DartelBundleCreateWarpProcess::operate< int64_t > );
    registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                     &DartelBundleCreateWarpProcess::operate< std::string > );

  }
  GKG_CATCH( "DartelBundleCreateWarpProcess::DartelBundleCreateWarpProcess( "
             "const std::string& theFileNameInTemplate, "
             "const std::string& theFileNameFlowField, "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameTemplate, "
             "const int32_t& theK, const bool& theInverse, "
             "const bool& theVerbose )" );

}


template < class L >
void DartelBundleCreateWarpProcess::operate( gkg::Process& process,
                                             const std::string& fileNameIn,
                                             const gkg::AnalyzedObject&,
                                             const std::string& )
{

  try
  {

    DartelBundleCreateWarpProcess& dartelCreateWarpProcess = 
                       static_cast< DartelBundleCreateWarpProcess& >( process );
    DartelBundleCreateWarpSubProcess< L > dartelSubProcess(
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
             "void DartelBundleCreateWarpProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
//   DartelBundleCreateWarpCommand
//


gkg::DartelBundleCreateWarpCommand::DartelBundleCreateWarpCommand(
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
  GKG_CATCH( "gkg::DartelBundleCreateWarpCommand::"
             "DartelBundleCreateWarpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DartelBundleCreateWarpCommand::DartelBundleCreateWarpCommand(
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
  GKG_CATCH( "gkg::DartelBundleCreateWarpCommand::"
             "DartelBundleCreateWarpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameInTemplate, "
             "const std::string& fileNameFlowField, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameTemplate, "
             "int32_t k, bool inverse, bool verbose )" );

}


gkg::DartelBundleCreateWarpCommand::DartelBundleCreateWarpCommand(
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
  GKG_CATCH( "gkg::DartelBundleCreateWarpCommand::"
             "DartelBundleCreateWarpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DartelBundleCreateWarpCommand::~DartelBundleCreateWarpCommand()
{
}


std::string gkg::DartelBundleCreateWarpCommand::getStaticName()
{

  try
  {

    return "DartelBundleCreateWarp";

  }
  GKG_CATCH( "std::string "
             "gkg::DartelBundleCreateWarpCommand::getStaticName()" );

}


void gkg::DartelBundleCreateWarpCommand::parse()
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
                                  "Warp of bundles",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Bundle map to (un)warp",
                                 fileNameIn );
    application.addSingleOption( "-i",
                                 "Input image used to create the bundles "
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
                                 "Warped or Unwarped bundles",
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
  GKG_CATCH_FUNCTOR( "void gkg::DartelBundleCreateWarpCommand::parse()" );

}


void gkg::DartelBundleCreateWarpCommand::execute(
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

    DartelBundleCreateWarpProcess dartelCreateWarpProcess( fileNameInTemplate,
                                                           fileNameFlowField,
                                                           fileNameOut,
                                                           fileNameTemplate,
                                                           k,
                                                           inverse,
                                                           verbose );
    dartelCreateWarpProcess.execute( fileNameIn );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelBundleCreateWarpCommand::execute( "
                     "const std::string& fileNameIn, "
                     "const std::string& fileNameInTemplate, "
                     "const std::string& fileNameFlowField, "
                     "const std::string& fileNameOut, "
                     "const std::string& fileNameTemplate, "
                     "int32_t k, bool inverse, bool verbose )" );

}


RegisterCommandCreator( DartelBundleCreateWarpCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameInTemplate ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameFlowField ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameTemplate ) +
    			DECLARE_INTEGER_PARAMETER_HELP( k ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( inverse ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
