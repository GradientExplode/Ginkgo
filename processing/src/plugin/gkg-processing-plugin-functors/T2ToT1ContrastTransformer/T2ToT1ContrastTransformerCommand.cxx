#include <gkg-processing-plugin-functors/T2ToT1ContrastTransformer/T2ToT1ContrastTransformerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class T2ToT1ContrastTransformerProcess : public gkg::Process
{

  public:

    T2ToT1ContrastTransformerProcess( const std::string& theFileNameOut,
                                      const double& theLowerThreshold,
                                      const double& theUpperThreshold,
                                      const bool& theAscii,
                                      const std::string& theFormat,
                                      const bool& theVerbose );

    const std::string& fileNameOut;
    const double& lowerThreshold;
    const double& upperThreshold;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void transform( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& );


};


T2ToT1ContrastTransformerProcess::T2ToT1ContrastTransformerProcess(
                                    const std::string& theFileNameOut,
                                    const double& theLowerThreshold,
                                    const double& theUpperThreshold,
                                    const bool& theAscii,
                                    const std::string& theFormat,
                                    const bool& theVerbose )
                                  : gkg::Process( "Volume" ),
                                    fileNameOut( theFileNameOut ),
                                    lowerThreshold( theLowerThreshold ),
                                    upperThreshold( theUpperThreshold ),
                                    ascii( theAscii ),
                                    format( theFormat ),
                                    verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                    &T2ToT1ContrastTransformerProcess::transform< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                    &T2ToT1ContrastTransformerProcess::transform< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                    &T2ToT1ContrastTransformerProcess::transform< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &T2ToT1ContrastTransformerProcess::transform< double > );

  }
  GKG_CATCH( "T2ToT1ContrastTransformerProcess::"
             "T2ToT1ContrastTransformerProcess( "
             "const std::string& theFileNameOut, "
             "const double& theLowerThreshold, "
             "const double& theUpperThreshold, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void T2ToT1ContrastTransformerProcess::transform(
                                                  gkg::Process& process,
                                                  const std::string& fileNameIn,
                                                  const gkg::AnalyzedObject&,
                                                  const std::string& )
{

  try
  {

    T2ToT1ContrastTransformerProcess&
      t2ToT1ContrastTransformerProcess = 
        static_cast< T2ToT1ContrastTransformerProcess& >( process );

    const std::string& fileNameOut =
      t2ToT1ContrastTransformerProcess.fileNameOut;
    const double& lowerThreshold =
      t2ToT1ContrastTransformerProcess.lowerThreshold;
    const double& upperThreshold =
      t2ToT1ContrastTransformerProcess.upperThreshold;
    const bool& ascii =
      t2ToT1ContrastTransformerProcess.ascii;
    const std::string& format =
      t2ToT1ContrastTransformerProcess.format;
    const bool& verbose =
      t2ToT1ContrastTransformerProcess.verbose;

    // reading data
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "transforming T2 to T1 contrast : " << std::flush;

    }

    typename gkg::Volume< T >::iterator
      i = volumeIn.begin(),
      ie = volumeIn.end();
    while ( i != ie )
    {

      if ( ( ( double )*i > lowerThreshold ) &&
           ( ( double )*i < upperThreshold ) )
      {

        *i = ( T )( -*i + lowerThreshold + upperThreshold );

      }
      else if ( ( double )*i >= upperThreshold )
      {

        *i = ( T )0;

      }
      ++ i;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      volumeIn,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void T2ToT1ContrastTransformerProcess::transform( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class T2ToT1ContrastTransformerCommand
//


gkg::T2ToT1ContrastTransformerCommand::T2ToT1ContrastTransformerCommand(
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
  GKG_CATCH( "gkg::T2ToT1ContrastTransformerCommand::"
             "T2ToT1ContrastTransformerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::T2ToT1ContrastTransformerCommand::T2ToT1ContrastTransformerCommand(
                                         const std::string& fileNameIn,
                      			 const std::string& fileNameOut,
                                         double lowerThreshold,
                                         double upperThreshold,
                      			 bool ascii,
                      			 const std::string& format,
                      			 bool verbose )
{

  try
  {

    execute( fileNameIn,
             fileNameOut, 
             lowerThreshold,
             upperThreshold,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::T2ToT1ContrastTransformerCommand::"
             "T2ToT1ContrastTransformerCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "double lowerThreshold, "
             "double upperThreshold, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::T2ToT1ContrastTransformerCommand::T2ToT1ContrastTransformerCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, double, lowerThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, double, upperThreshold );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut, 
             lowerThreshold,
             upperThreshold,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::T2ToT1ContrastTransformerCommand::"
             "T2ToT1ContrastTransformerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::T2ToT1ContrastTransformerCommand::~T2ToT1ContrastTransformerCommand()
{
}


std::string gkg::T2ToT1ContrastTransformerCommand::getStaticName()
{

  try
  {

    return "T2ToT1ContrastTransformer";

  }
  GKG_CATCH( "std::string "
             "gkg::T2ToT1ContrastTransformerCommand::getStaticName()" );

}


void gkg::T2ToT1ContrastTransformerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    double lowerThreshold = 0.0;
    double upperThreshold = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "T2 to T1 contrast transformer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-lowerThreshold",
                                 "Lower threshold",
                                 lowerThreshold );
    application.addSingleOption( "-upperThreshold",
                                 "Upper threshold",
                                 upperThreshold );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
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

    execute( fileNameIn,
             fileNameOut, 
             lowerThreshold,
             upperThreshold,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::T2ToT1ContrastTransformerCommand::parse()" );

}


void gkg::T2ToT1ContrastTransformerCommand::execute(
                                     const std::string& fileNameIn,
                      		     const std::string& fileNameOut,
                                     double lowerThreshold,
                                     double upperThreshold,
                      		     bool ascii,
                      		     const std::string& format,
                      		     bool verbose )
{

  try
  {

    // sanity checks(s)
    if (lowerThreshold >= upperThreshold )
    {

      throw std::runtime_error( 
                       "lower threshold must be greater than upper threshold" );

    } 

    //
    // launching T2 to T1 transformer process
    //
    T2ToT1ContrastTransformerProcess 
      t2ToT1ContrastTransformerProcess( fileNameOut,
                                        lowerThreshold,
                                        upperThreshold,
                                        ascii,
                                        format,
                                        verbose );

    t2ToT1ContrastTransformerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::T2ToT1ContrastTransformerCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "double lowerThreshold, "
             "double upperThreshold, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( T2ToT1ContrastTransformerCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_FLOATING_PARAMETER_HELP( lowerThreshold ) +
                        DECLARE_FLOATING_PARAMETER_HELP( upperThreshold ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
