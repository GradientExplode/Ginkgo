#include <gkg-processing-plugin-functors/HoleFilling2d/HoleFilling2dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-morphology/HoleFilling2d.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/OutsideOrEqualToFunction_i.h>
#include <gkg-processing-algobase/OutsideFunction_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   HoleFilling2dProcess
//


class HoleFilling2dProcess : public gkg::Process
{

  public:

    HoleFilling2dProcess( const std::string& fileNameOut,
                          const std::string& outType,
                          const std::string& mode,
                          const double& threshold1,
                          const double& threshold2,
                          const double& foreground,
                          const double& background,
                          const bool& ascii,
                          const std::string& format,
                          const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


HoleFilling2dProcess::HoleFilling2dProcess( const std::string& fileNameOut,
                                            const std::string& outType,
                                            const std::string& mode,
                                            const double& threshold1,
                                            const double& threshold2,
                                            const double& foreground,
                                            const double& background,
                                            const bool& ascii,
                                            const std::string& format,
                                            const bool& verbose)
                  : gkg::Process(),
                    _fileNameOut( fileNameOut ),
                    _outType( outType ),
                    _mode( mode ),
                    _threshold1( threshold1 ),
                    _threshold2( threshold2 ),
                    _foreground( foreground ),
                    _background( background ),
                    _ascii( ascii ),
                    _format( format ),
                    _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &HoleFilling2dProcess::operate< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &HoleFilling2dProcess::operate< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &HoleFilling2dProcess::operate< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &HoleFilling2dProcess::operate< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &HoleFilling2dProcess::operate< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &HoleFilling2dProcess::operate< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &HoleFilling2dProcess::operate< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &HoleFilling2dProcess::operate< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &HoleFilling2dProcess::operate< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &HoleFilling2dProcess::operate< double > );

}


const std::string& HoleFilling2dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& HoleFilling2dProcess::getOutType() const
{

  return _outType;

}


const std::string& HoleFilling2dProcess::getMode() const
{

  return _mode;

}


const double& HoleFilling2dProcess::getThreshold1() const
{

  return _threshold1;

}


const double& HoleFilling2dProcess::getThreshold2() const
{

  return _threshold2;

}


const double& HoleFilling2dProcess::getForeground() const
{

  return _foreground;

}


const double& HoleFilling2dProcess::getBackground() const
{

  return _background;

}


const bool& HoleFilling2dProcess::getAscii() const
{

  return _ascii;

}


const std::string& HoleFilling2dProcess::getFormat() const
{

  return _format;

}


const bool& HoleFilling2dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void HoleFilling2dProcess::operate( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    HoleFilling2dProcess&
      holeFilling2dProcess = static_cast< HoleFilling2dProcess& >( process );

    // reading data
    if ( holeFilling2dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( holeFilling2dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    gkg::TestFunction< T >* testFunction = 0;
    if ( holeFilling2dProcess.getMode() == "lt" )
    {

      testFunction = new gkg::LowerThanFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "le" )
    {

      testFunction = new gkg::LowerOrEqualToFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "eq" )
    {

      testFunction = new gkg::EqualToFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "di" )
    {

      testFunction = new gkg::DifferentFromFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "ge" )
    {

      testFunction = new gkg::GreaterOrEqualToFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "gt" )
    {

      testFunction = new gkg::GreaterThanFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1() );

    }
    else if ( holeFilling2dProcess.getMode() == "be" )
    {

      testFunction = new gkg::BetweenOrEqualToFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1(),
                                    ( T )holeFilling2dProcess.getThreshold2() );

    }
    else if ( holeFilling2dProcess.getMode() == "bt" )
    {

      testFunction = new gkg::BetweenFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1(),
                                    ( T )holeFilling2dProcess.getThreshold2() );

    }
    else if ( holeFilling2dProcess.getMode() == "oe" )
    {

      testFunction = new gkg::OutsideOrEqualToFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1(),
                                    ( T )holeFilling2dProcess.getThreshold2() );

    }
    else if ( holeFilling2dProcess.getMode() == "ou" )
    {

      testFunction = new gkg::OutsideFunction< T >(
                                    ( T )holeFilling2dProcess.getThreshold1(),
                                    ( T )holeFilling2dProcess.getThreshold2() );

    }

    std::string outType = holeFilling2dProcess.getOutType();
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;

      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint8_t > > 
        binarizer( *testFunction,
                   ( uint8_t )holeFilling2dProcess.getForeground(),
                   ( uint8_t )holeFilling2dProcess.getBackground() );

      binarizer.binarize( volumeIn, volumeOut );

      gkg::HoleFilling2d< uint8_t >
        holeFilling2d( ( uint8_t )holeFilling2dProcess.getForeground(),
                       ( uint8_t )holeFilling2dProcess.getBackground() );
      holeFilling2d.fill( volumeOut, volumeOut );

      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << holeFilling2dProcess.getFileNameOut() << "' : " 
                  << std::flush;

      }
      gkg::Writer::getInstance().write( holeFilling2dProcess.getFileNameOut(),
                                        volumeOut,
                                        holeFilling2dProcess.getAscii(),
                                        holeFilling2dProcess.getFormat() );
      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;

      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int16_t > > 
        binarizer( *testFunction,
                   ( int16_t )holeFilling2dProcess.getForeground(),
                   ( int16_t )holeFilling2dProcess.getBackground() );

      binarizer.binarize( volumeIn, volumeOut );

      gkg::HoleFilling2d< int16_t >
        holeFilling2d( ( int16_t )holeFilling2dProcess.getForeground(),
                       ( int16_t )holeFilling2dProcess.getBackground() );
      holeFilling2d.fill( volumeOut, volumeOut );

      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << holeFilling2dProcess.getFileNameOut() << "' : " 
                  << std::flush;

      }
      gkg::Writer::getInstance().write( holeFilling2dProcess.getFileNameOut(),
                                        volumeOut,
                                        holeFilling2dProcess.getAscii(),
                                        holeFilling2dProcess.getFormat() );
      if ( holeFilling2dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void HoleFilling2dProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   HoleFilling2dCommand
//


gkg::HoleFilling2dCommand::HoleFilling2dCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::HoleFilling2dCommand::HoleFilling2dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::HoleFilling2dCommand::HoleFilling2dCommand( const std::string& fileNameIn,
                          			 const std::string& fileNameOut,
                          			 const std::string& outType,
                          			 const std::string& mode,
                          			 double threshold1,
                          			 double threshold2,
                          			 double foreground,
                          			 double background,
                          			 bool ascii,
                          			 const std::string& format,
                          			 bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outType, mode, threshold1, threshold2,
             foreground, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::HoleFilling2dCommand::HoleFilling2dCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::HoleFilling2dCommand::HoleFilling2dCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, foreground );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outType, mode, threshold1, threshold2,
             foreground, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::HoleFilling2dCommand::HoleFilling2dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::HoleFilling2dCommand::~HoleFilling2dCommand()
{
}


std::string gkg::HoleFilling2dCommand::getStaticName()
{

  try
  {

    return "HoleFilling2d";

  }
  GKG_CATCH( "std::string gkg::HoleFilling2dCommand::getStaticName()" );

}


void gkg::HoleFilling2dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double foreground = 1.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Hole Filling 2d",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type uint8_t/int16_t "
                                 "(default=int16_t)",
                                 outType,
                                 true );
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
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
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

    execute( fileNameIn, fileNameOut, outType, mode, threshold1, threshold2,
             foreground, background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::HoleFilling2dCommand::parse()" );

}


void gkg::HoleFilling2dCommand::execute( const std::string& fileNameIn,
                          		 const std::string& fileNameOut,
                          		 const std::string& outType,
                          		 const std::string& mode,
                          		 double threshold1,
                          		 double threshold2,
                          		 double foreground,
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
         
    if ( !outType.empty() &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    //
    // launching process
    //
    HoleFilling2dProcess holeFilling2dProcess( fileNameOut,
                                               outType,
                                               mode,
                                               threshold1,
                                               threshold2,
                                               foreground,
                                               background,
                                               ascii,
                                               format,
                                               verbose );
    holeFilling2dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::HoleFilling2dCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( HoleFilling2dCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( outType ) +
                        DECLARE_STRING_PARAMETER_HELP( mode ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                        DECLARE_FLOATING_PARAMETER_HELP( foreground ) +
                        DECLARE_FLOATING_PARAMETER_HELP( background ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
