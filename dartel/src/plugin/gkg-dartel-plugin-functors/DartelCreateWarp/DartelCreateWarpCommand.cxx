#include <gkg-dartel-plugin-functors/DartelCreateWarp/DartelCreateWarpCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dartel-volume/DartelVolumeTransformation.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class DartelCreateWarpProcess : public gkg::Process
{

  public:

    DartelCreateWarpProcess( const std::string& theFileNameFlowField,
                             const std::string& theFileNameOut,
                             const std::string& theFileNameTemplate,
                             const int32_t& theK,
                             const int32_t& theDegree,
                             const bool& theInverse,
                             const bool& theVerbose );

    const std::string& fileNameFlowField;
    const std::string& fileNameOut;
    const std::string& fileNameTemplate;
    const int32_t& k;
    const int32_t& degree;
    const bool& inverse;
    const bool& verbose;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


DartelCreateWarpProcess::DartelCreateWarpProcess(
                             		const std::string& theFileNameFlowField,
                             		const std::string& theFileNameOut,
                             		const std::string& theFileNameTemplate,
                             		const int32_t& theK,
                             		const int32_t& theDegree,
                             		const bool& theInverse,
                             		const bool& theVerbose )
                       : gkg::Process(),
                         fileNameFlowField( theFileNameFlowField ),
                         fileNameOut( theFileNameOut ),
                         fileNameTemplate( theFileNameTemplate ),
                         k( theK ),
                         degree( theDegree ),
                         inverse( theInverse ),
                         verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &DartelCreateWarpProcess::operate< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &DartelCreateWarpProcess::operate< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &DartelCreateWarpProcess::operate< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &DartelCreateWarpProcess::operate< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &DartelCreateWarpProcess::operate< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &DartelCreateWarpProcess::operate< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &DartelCreateWarpProcess::operate< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &DartelCreateWarpProcess::operate< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DartelCreateWarpProcess::operate< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DartelCreateWarpProcess::operate< double > );

  }
  GKG_CATCH( "DartelCreateWarpProcess::DartelCreateWarpProcess( "
             "const std::string& theFileNameFlowField, "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameTemplate, "
             "const int32_t& theK, const int32_t& theDegree, "
             "const bool& theInverse, const bool& theVerbose )" );

}


template < class T >
void DartelCreateWarpProcess::operate( gkg::Process& process,
                                       const std::string& fileNameIn,
                                       const gkg::AnalyzedObject&,
                                       const std::string& )
{

  try
  {

    DartelCreateWarpProcess& dartelCreateWarpProcess = 
                             static_cast< DartelCreateWarpProcess& >( process );

    gkg::Volume< T > inVolume;
    gkg::Volume< float > flowfield;
    gkg::Volume< T > templateProxy;
    gkg::Volume< T > outVolume;

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "reading input image : " << fileNameIn;

    }
    gkg::Reader::getInstance().read( fileNameIn, inVolume );
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

    gkg::DartelVolumeTransformation< T, float > dartel( 
                                               dartelCreateWarpProcess.k,
                                               dartelCreateWarpProcess.degree );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "applying deformation" << std::flush;

    }
    if ( dartelCreateWarpProcess.inverse )
    {

      dartel.unwrap( inVolume, flowfield, templateProxy, outVolume );

    }
    else
    {

      dartel.wrap( inVolume, flowfield, outVolume );

    }
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << "writing output image : "
                << dartelCreateWarpProcess.fileNameOut << std::flush;

    }
    gkg::Writer::getInstance().write( dartelCreateWarpProcess.fileNameOut,
                                      outVolume );
    if ( dartelCreateWarpProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DartelCreateWarpProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
//   DartelCreateWarpCommand
//


gkg::DartelCreateWarpCommand::DartelCreateWarpCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DartelCreateWarpCommand::DartelCreateWarpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DartelCreateWarpCommand::DartelCreateWarpCommand(
                                          const std::string& fileNameIn,
                                          const std::string& fileNameFlowField,
                                          const std::string& fileNameOut,
                                          const std::string& fileNameTemplate,
                                          int32_t k,
                                          int32_t degree,
                                          bool inverse,
                                          bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameFlowField, fileNameOut, fileNameTemplate, 
             k, degree, inverse, verbose );

  }
  GKG_CATCH( "gkg::DartelCreateWarpCommand::DartelCreateWarpCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameFlowField, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameTemplate, "
             "int32_t k, int32_t degree, bool inverse, bool verbose )" );

}


gkg::DartelCreateWarpCommand::DartelCreateWarpCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFlowField );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTemplate );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, k );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, degree );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, inverse );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameFlowField, fileNameOut, fileNameTemplate, 
             k, degree, inverse, verbose );

  }
  GKG_CATCH( "gkg::DartelCreateWarpCommand::DartelCreateWarpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DartelCreateWarpCommand::~DartelCreateWarpCommand()
{
}


std::string gkg::DartelCreateWarpCommand::getStaticName()
{

  try
  {

    return "DartelCreateWarp";

  }
  GKG_CATCH( "std::string gkg::DartelCreateWarpCommand::getStaticName()" );

}


void gkg::DartelCreateWarpCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameFlowField;
    std::string fileNameOut;
    std::string fileNameTemplate = "";
    int32_t k = 6;
    int32_t degree = 1;
    bool inverse = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Dartel Create direct or inverse "
                                  "Warp of volumes",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Image to (un)warp",
                                 fileNameIn );
    application.addSingleOption( "-u",
                                 "Flow field",
                                 fileNameFlowField );
    application.addSingleOption( "-k",
                                 "Log2 of the Euler time steps (default=6)",
                                 k,
                                 true );
    application.addSingleOption( "-d",
                                 "Degree of the interpolation spline [0-7] "
                                 "(default=1)",
                                 degree,
                                 true );
    application.addSingleOption( "-inverse",
                                 "Create inverse warp",
                                 inverse,
                                 true );
    application.addSingleOption( "-o",
                                 "Warped or Unwarped volumes",
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

    execute( fileNameIn, fileNameFlowField, fileNameOut, fileNameTemplate, 
             k, degree, inverse, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelCreateWarpCommand::parse()" );

}


void gkg::DartelCreateWarpCommand::execute(
                                           const std::string& fileNameIn,
                                           const std::string& fileNameFlowField,
                                           const std::string& fileNameOut,
                                           const std::string& fileNameTemplate,
                                           int32_t k,
                                           int32_t degree,
                                           bool inverse,
                                           bool verbose )
{

  try
  {

    if ( degree < 0 )
    {

      degree = 0;

    }
    else if ( degree > 7 )
    {

      degree = 7;

    }

    DartelCreateWarpProcess dartelCreateWarpProcess( fileNameFlowField,
                                                     fileNameOut,
                                                     fileNameTemplate,
                                                     k,
                                                     degree,
                                                     inverse,
                                                     verbose );
    dartelCreateWarpProcess.execute( fileNameIn );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelCreateWarpCommand::execute( "
                     "const std::string& fileNameIn, "
                     "const std::string& fileNameFlowField, "
                     "const std::string& fileNameOut, "
                     "const std::string& fileNameTemplate, "
                     "int32_t k, int32_t degree, bool inverse, "
                     "bool verbose )" );

}


RegisterCommandCreator( DartelCreateWarpCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameFlowField ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameTemplate ) +
    			DECLARE_INTEGER_PARAMETER_HELP( k ) +
    			DECLARE_INTEGER_PARAMETER_HELP( degree ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( inverse ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
