#include <gkg-dartel-plugin-functors/DartelFlowFieldToVectorField/DartelFlowFieldToVectorFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-dartel-core/DartelExponentiation.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class DartelVectorFieldProcess : public gkg::Process
{

  public:

    DartelVectorFieldProcess( const std::string& theFileNameOut,
                              const int32_t& theK,
                              const bool& theVerbose );

    const std::string& fileNameOut;
    const int32_t& k;
    const bool& verbose;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


DartelVectorFieldProcess::DartelVectorFieldProcess(
                                              const std::string& theFileNameOut,
                             		      const int32_t& theK,
                             		      const bool& theVerbose )
                        : gkg::Process(),
                          fileNameOut( theFileNameOut ),
                          k( theK ),
                          verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DartelVectorFieldProcess::operate< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DartelVectorFieldProcess::operate< double > );

  }
  GKG_CATCH( "DartelVectorFieldProcess::DartelVectorFieldProcess( "
             "const std::string& theFileNameOut, "
             "const int32_t& theK, const bool& theVerbose )" );

}


template < class T >
void DartelVectorFieldProcess::operate( gkg::Process& process,
                                        const std::string& fileNameIn,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    DartelVectorFieldProcess& dartelVectorFieldProcess = 
                            static_cast< DartelVectorFieldProcess& >( process );

    gkg::Volume< T > flowField;
    gkg::Volume< T > outVolume;
    std::string fileNameOut;

    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << "reading Dartel flow field : " << fileNameIn;

    }
    std::string format = gkg::Reader::getInstance().read( fileNameIn,
                                                          flowField );
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    gkg::DartelExponentiation< T > dartel( dartelVectorFieldProcess.k );
    gkg::Volume< T > directVectorField;
    gkg::Volume< T > inverseVectorField;

    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << "computing direct vector field : " << fileNameIn;

    }
    dartel.getDirectVectorField( flowField, directVectorField );
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << "computing inverse vector field : " << fileNameIn;

    }
    dartel.getInverseVectorField( flowField, inverseVectorField );
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    gkg::File file( dartelVectorFieldProcess.fileNameOut );

    fileNameOut = file.getDirectoryName() + "direct_" + file.getBaseName(); 
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << "writing direct vector field : " 
                << fileNameOut << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, directVectorField,
                                      false, format );
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

    fileNameOut = file.getDirectoryName() + "inverse_" + file.getBaseName(); 
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << "writing inverse vector field : " 
                << fileNameOut << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, inverseVectorField,
                                      false, format );
    if ( dartelVectorFieldProcess.verbose )
    {

      std::cout << " - done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DartelVectorFieldProcess::operate( "
             "gkg::Process& process, const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, const std::string& )" );

}


//
//   DartelFlowFieldToVectorFieldCommand
//


gkg::DartelFlowFieldToVectorFieldCommand::DartelFlowFieldToVectorFieldCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                        : gkg::Command( argc, argv,
                                                        loadPlugin,
                                                        removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DartelFlowFieldToVectorFieldCommand::"
             "DartelFlowFieldToVectorFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DartelFlowFieldToVectorFieldCommand::DartelFlowFieldToVectorFieldCommand(
                                           const std::string& fileNameFlowField,
                                           const std::string& fileNameOut,
                                           int32_t k,
                                           bool verbose )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameFlowField, fileNameOut, k, verbose );

  }
  GKG_CATCH( "gkg::DartelFlowFieldToVectorFieldCommand::"
             "DartelFlowFieldToVectorFieldCommand( "
             "const std::string& fileNameFlowField, "
             "const std::string& fileNameOut, int32_t k, bool verbose )" );

}


gkg::DartelFlowFieldToVectorFieldCommand::DartelFlowFieldToVectorFieldCommand(
                                             const gkg::Dictionary& parameters )
                                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFlowField );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, k );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameFlowField, fileNameOut, k, verbose );

  }
  GKG_CATCH( "gkg::DartelFlowFieldToVectorFieldCommand::"
             "DartelFlowFieldToVectorFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DartelFlowFieldToVectorFieldCommand::~DartelFlowFieldToVectorFieldCommand()
{
}


std::string gkg::DartelFlowFieldToVectorFieldCommand::getStaticName()
{

  try
  {

    return "DartelFlowFieldToVectorField";

  }
  GKG_CATCH( "std::string "
             "gkg::DartelFlowFieldToVectorFieldCommand::getStaticName()" );

}


void gkg::DartelFlowFieldToVectorFieldCommand::parse()
{

  try
  {

    std::string fileNameFlowField;
    std::string fileNameOut;
    int32_t k = 6;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Generate direct and inverse deformation " 
                                  "vector fields from a Dartel flow field",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Flow field",
                                 fileNameFlowField );
    application.addSingleOption( "-k",
                                 "Log2 of the Euler time steps (default=6)",
                                 k,
                                 true );
    application.addSingleOption( "-o",
                                 "Vector fields base name "
                                 "(default=same as input)",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameFlowField, fileNameOut, k, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelFlowFieldToVectorFieldCommand::parse()" );

}


void gkg::DartelFlowFieldToVectorFieldCommand::execute(
                                           const std::string& fileNameFlowField,
                                           const std::string& fileNameOut,
                                           int32_t k,
                                           bool verbose )
{

  try
  {

    std::string baseOutFileName = fileNameOut;

    if ( fileNameOut.empty() )
    {

      baseOutFileName = fileNameFlowField;

    }

    DartelVectorFieldProcess dartelVectorFieldProcess( baseOutFileName,
                                                       k,
                                                       verbose );
    dartelVectorFieldProcess.execute( fileNameFlowField );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DartelFlowFieldToVectorFieldCommand::execute( "
                     "const std::string& fileNameFlowField, "
                     "const std::string& fileNameOut, "
                     "int32_t k, bool verbose )" );

}


RegisterCommandCreator( DartelFlowFieldToVectorFieldCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameFlowField ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( k ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
