#include <gkg-processing-plugin-functors/StatisticAnalyzer/StatisticAnalyzerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   StatisticAnalyzerProcess
//


class StatisticAnalyzerProcess : public gkg::Process
{

  public:

    StatisticAnalyzerProcess();

  private:

    template < class T >
    static void analyze( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


StatisticAnalyzerProcess::StatisticAnalyzerProcess()
                         : gkg::Process()
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &StatisticAnalyzerProcess::analyze< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &StatisticAnalyzerProcess::analyze< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &StatisticAnalyzerProcess::analyze< double > );

}


template < class T >
void StatisticAnalyzerProcess::analyze( gkg::Process&,
                                        const std::string& fileNameIn,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    // reading data
    std::cout << "reading '" << fileNameIn << "' : " << std::flush;
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    std::cout << std::endl;

    gkg::StatisticAnalyzer< gkg::Volume< T > > analyzer;
    std::cout << "average : " << analyzer.getAverage( volume ) << std::endl; 
    std::cout << "stddev  : " << analyzer.getStdDev( volume ) << std::endl; 
    std::cout << "minimum : " << analyzer.getDoubleMinimum( volume )
              << std::endl; 
    std::cout << "maximum : " << analyzer.getDoubleMaximum( volume )
              << std::endl; 
    std::cout << "corrected maximum : "
              << analyzer.getDoubleCorrectedMaximum( volume )
              << std::endl; 
    std::cout << "noise average : " << analyzer.getNoiseAverage( volume )
              << std::endl; 
    std::cout << "noise stddev  : " << analyzer.getNoiseStdDev( volume )
              << std::endl; 
    std::cout << "automatic threshold  : "
              << analyzer.getDoubleAutomaticThreshold( volume )
              << std::endl; 

  }
  GKG_CATCH( "template < class T > "
             "void StatisticAnalyzerProcess::analyze( gkg::Process&, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   StatisticAnalyzerCommand
//


gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand( int32_t argc,
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
  GKG_CATCH( "gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand(
                                                 const std::string& fileNameIn )
                             : gkg::Command()
{

  try
  {

    execute( fileNameIn );

  }
  GKG_CATCH( "gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand( "
             "const std::string& fileNameIn )" );

}


gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    
    execute( fileNameIn );

  }
  GKG_CATCH( "gkg::StatisticAnalyzerCommand::StatisticAnalyzerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::StatisticAnalyzerCommand::~StatisticAnalyzerCommand()
{
}


std::string gkg::StatisticAnalyzerCommand::getStaticName()
{

  try
  {

    return "StatisticAnalyzer";

  }
  GKG_CATCH( "std::string gkg::StatisticAnalyzerCommand::getStaticName()" );

}


void gkg::StatisticAnalyzerCommand::parse()
{

  try
  {

    std::string fileNameIn;

    gkg::Application application( _argc, _argv,
                                  "Statistic analysis of a container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );

    application.initialize();

    execute( fileNameIn );

  }
  GKG_CATCH_FUNCTOR( "void gkg::StatisticAnalyzerCommand::parse()" );

}


void gkg::StatisticAnalyzerCommand::execute( const std::string& fileNameIn )
{

  try
  {

    StatisticAnalyzerProcess statisticAnalyzerProcess;
    statisticAnalyzerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::StatisticAnalyzerCommand::execute( "
             "const std::string& fileNameIn )" );

}


RegisterCommandCreator( StatisticAnalyzerCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) );
