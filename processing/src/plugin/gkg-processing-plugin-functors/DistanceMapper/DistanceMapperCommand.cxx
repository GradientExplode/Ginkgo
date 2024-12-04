#include <gkg-processing-plugin-functors/DistanceMapper/DistanceMapperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/MaurerDistanceMapper.h>
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


class DistanceMapperProcess : public gkg::Process
{

  public:

    DistanceMapperProcess( const std::string& fileNameOut,
                           const std::string& mode,
                           const double& threshold1,
                           const double& threshold2,
                           const bool& ascii,
                           const std::string& format,
                           const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void distanceMap( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& );

    const std::string& _fileNameOut;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


DistanceMapperProcess::DistanceMapperProcess( const std::string& fileNameOut,
                                              const std::string& mode,
                                              const double& threshold1,
                                              const double& threshold2,
                                              const bool& ascii,
                                              const std::string& format,
                                              const bool& verbose )
                      : gkg::Process(),
                        _fileNameOut( fileNameOut ),
                        _mode( mode ),
                        _threshold1( threshold1 ),
                        _threshold2( threshold2 ),
                        _ascii( ascii ),
                        _format( format ),
                        _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &DistanceMapperProcess::distanceMap< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &DistanceMapperProcess::distanceMap< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &DistanceMapperProcess::distanceMap< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &DistanceMapperProcess::distanceMap< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &DistanceMapperProcess::distanceMap< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &DistanceMapperProcess::distanceMap< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &DistanceMapperProcess::distanceMap< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &DistanceMapperProcess::distanceMap< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &DistanceMapperProcess::distanceMap< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &DistanceMapperProcess::distanceMap< double > );

}


const std::string& DistanceMapperProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& DistanceMapperProcess::getMode() const
{

  return _mode;

}


const double& DistanceMapperProcess::getThreshold1() const
{

  return _threshold1;

}


const double& DistanceMapperProcess::getThreshold2() const
{

  return _threshold2;

}


const bool& DistanceMapperProcess::getAscii() const
{

  return _ascii;

}


const std::string& DistanceMapperProcess::getFormat() const
{

  return _format;

}


const bool& DistanceMapperProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void DistanceMapperProcess::distanceMap( gkg::Process& process,
                                         const std::string& fileNameIn,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    DistanceMapperProcess&
      distanceMapperProcess = static_cast< DistanceMapperProcess& >( process );

    // reading data
    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "processing distance map : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 0;
    if ( distanceMapperProcess.getMode() == "lt" )
    {

      testFunction = new gkg::LowerThanFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "le" )
    {

      testFunction = new gkg::LowerOrEqualToFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "eq" )
    {

      testFunction = new gkg::EqualToFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "di" )
    {

      testFunction = new gkg::DifferentFromFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "ge" )
    {

      testFunction = new gkg::GreaterOrEqualToFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "gt" )
    {

      testFunction = new gkg::GreaterThanFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1() );

    }
    else if ( distanceMapperProcess.getMode() == "be" )
    {

      testFunction = new gkg::BetweenOrEqualToFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1(),
                                   ( T )distanceMapperProcess.getThreshold2() );

    }
    else if ( distanceMapperProcess.getMode() == "bt" )
    {

      testFunction = new gkg::BetweenFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1(),
                                   ( T )distanceMapperProcess.getThreshold2() );

    }
    else if ( distanceMapperProcess.getMode() == "oe" )
    {

      testFunction = new gkg::OutsideOrEqualToFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1(),
                                   ( T )distanceMapperProcess.getThreshold2() );

    }
    else if ( distanceMapperProcess.getMode() == "ou" )
    {

      testFunction = new gkg::OutsideFunction< T >(
                                   ( T )distanceMapperProcess.getThreshold1(),
                                   ( T )distanceMapperProcess.getThreshold2() );

    }


    gkg::Volume< float > volumeOut;
    gkg::MaurerDistanceMapper< T > distanceMapper( *testFunction );
    distanceMapper.getDistance( volumeIn, volumeOut,
                                distanceMapperProcess.getVerbose() );

    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "writing '"
                << distanceMapperProcess.getFileNameOut() << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( distanceMapperProcess.getFileNameOut(),
                                      volumeOut,
                                      distanceMapperProcess.getAscii(),
                                      distanceMapperProcess.getFormat() );
    if ( distanceMapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DistanceMapperProcess::distanceMap( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   DistanceMapperCommand
//


gkg::DistanceMapperCommand::DistanceMapperCommand( int32_t argc,
                                 		   char* argv[],
                                 		   bool loadPlugin,
                                 		   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DistanceMapperCommand::DistanceMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DistanceMapperCommand::DistanceMapperCommand(
                           			 const std::string& fileNameIn,
                           			 const std::string& fileNameOut,
                           			 const std::string& mode,
                           			 double threshold1,
                           			 double threshold2,
                           			 bool ascii,
                           			 const std::string& format,
                           			 bool verbose )
                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, mode, threshold1, threshold2, 
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DistanceMapperCommand::DistanceMapperCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::DistanceMapperCommand::DistanceMapperCommand(
                                             const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, mode, threshold1, threshold2, 
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DistanceMapperCommand::DistanceMapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DistanceMapperCommand::~DistanceMapperCommand()
{
}


std::string gkg::DistanceMapperCommand::getStaticName()
{

  try
  {

    return "DistanceMapper";

  }
  GKG_CATCH( "std::string gkg::DistanceMapperCommand::getStaticName()" );

}


void gkg::DistanceMapperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Conditional distance mapper "
                                  "of container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-m",
                                 "Condition mode\n"
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
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DistanceMapperCommand::parse()" );

}


void gkg::DistanceMapperCommand::execute( const std::string& fileNameIn,
                          		  const std::string& fileNameOut,
                          		  const std::string& mode,
                          		  double threshold1,
                          		  double threshold2,
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

      throw std::runtime_error( "invalid condition mode" );

    }
         
    //
    // launching process
    //
    DistanceMapperProcess distanceMapperProcess( fileNameOut,
                                                 mode,
                                                 threshold1,
                                                 threshold2,
                                                 ascii,
                                                 format,
                                                 verbose );
    distanceMapperProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::DistanceMapperCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( DistanceMapperCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( mode ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
