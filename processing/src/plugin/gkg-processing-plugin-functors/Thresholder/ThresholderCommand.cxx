#include <gkg-processing-plugin-functors/Thresholder/ThresholderCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class ThresholderProcess : public gkg::Process
{

  public:

    ThresholderProcess( const std::string& fileNameOut,
                        const std::string& mode,
                        const double& threshold1,
                        const double& threshold2,
                        const double& background,
                        const bool& ascii,
                        const std::string& format,
                        const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void threshold( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& );

    const std::string& _fileNameOut;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


ThresholderProcess::ThresholderProcess( const std::string& fileNameOut,
                                        const std::string& mode,
                                        const double& threshold1,
                                        const double& threshold2,
                                        const double& background,
                                        const bool& ascii,
                                        const std::string& format,
                                        const bool& verbose)
                   : gkg::Process( "Volume" ),
                     _fileNameOut( fileNameOut ),
                     _mode( mode ),
                     _threshold1( threshold1 ),
                     _threshold2( threshold2 ),
                     _background( background ),
                     _ascii( ascii ),
                     _format( format ),
                     _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &ThresholderProcess::threshold< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &ThresholderProcess::threshold< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &ThresholderProcess::threshold< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &ThresholderProcess::threshold< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &ThresholderProcess::threshold< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &ThresholderProcess::threshold< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &ThresholderProcess::threshold< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &ThresholderProcess::threshold< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &ThresholderProcess::threshold< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &ThresholderProcess::threshold< double > );

}


const std::string& ThresholderProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& ThresholderProcess::getMode() const
{

  return _mode;

}


const double& ThresholderProcess::getThreshold1() const
{

  return _threshold1;

}


const double& ThresholderProcess::getThreshold2() const
{

  return _threshold2;

}


const double& ThresholderProcess::getBackground() const
{

  return _background;

}


const bool& ThresholderProcess::getAscii() const
{

  return _ascii;

}


const std::string& ThresholderProcess::getFormat() const
{

  return _format;

}


const bool& ThresholderProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void ThresholderProcess::threshold( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    ThresholderProcess&
      thresholderProcess = static_cast< ThresholderProcess& >( process );

    // reading data
    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "thresholding : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                      thresholderProcess.getMode(),
			              ( T )thresholderProcess.getThreshold1(),
                                      ( T )thresholderProcess.getThreshold2() );
    
    gkg::Thresholder< gkg::Volume< T > >
      thresholder( *testFunction, ( T )thresholderProcess.getBackground() );
    thresholder.threshold( volume, volume );

    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "writing '"
                << thresholderProcess.getFileNameOut() << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( thresholderProcess.getFileNameOut(),
                                      volume,
                                      thresholderProcess.getAscii(),
                                      thresholderProcess.getFormat() );
    if ( thresholderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
    
    delete testFunction;

  }
  GKG_CATCH( "template < class T > "
             "void ThresholderProcess::threshold( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   ThresholderCommand
//


gkg::ThresholderCommand::ThresholderCommand( int32_t argc,
                                 	     char* argv[],
                                 	     bool loadPlugin,
                                 	     bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::ThresholderCommand::ThresholderCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::ThresholderCommand::ThresholderCommand( const std::string& fileNameIn,
                        		     const std::string& fileNameOut,
                        		     const std::string& mode,
                        		     double threshold1,
                        		     double threshold2,
                        		     double background,
                        		     bool ascii,
                        		     const std::string& format,
                        		     bool verbose )
                       : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, mode, threshold1, threshold2,
             background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ThresholderCommand::ThresholderCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::ThresholderCommand::ThresholderCommand( const gkg::Dictionary& parameters )
                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, mode, threshold1, threshold2,
             background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ThresholderCommand::ThresholderCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ThresholderCommand::~ThresholderCommand()
{
}


std::string gkg::ThresholderCommand::getStaticName()
{

  try
  {

    return "Thresholder";

  }
  GKG_CATCH( "std::string gkg::ThresholderCommand::getStaticName()" );

}


void gkg::ThresholderCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Threshold operation on container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
                                 true );
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

    execute( fileNameIn, fileNameOut, mode, threshold1, threshold2,
             background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::ThresholderCommand::parse()" );

}


void gkg::ThresholderCommand::execute( const std::string& fileNameIn,
                        	       const std::string& fileNameOut,
                        	       const std::string& mode,
                        	       double threshold1,
                        	       double threshold2,
                        	       double background,
                        	       bool ascii,
                        	       const std::string& format,
                        	       bool verbose )
{

  try
  {

    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid threshold mode" );

    }

    //
    // launching process
    //
    ThresholderProcess thresholderProcess( fileNameOut,
                                           mode,
                                           threshold1,
                                           threshold2,
                                           background,
                                           ascii,
                                           format,
                                           verbose );
    thresholderProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::ThresholderCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( ThresholderCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( mode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
