#include <gkg-processing-plugin-functors/TopologyClassifier/TopologyClassifierCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-topology/TopologyClassifier_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class TopologyClassifierProcess : public gkg::Process
{

  public:

    TopologyClassifierProcess( const std::string& fileNameOut,
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
    static void classification( gkg::Process& process,
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


TopologyClassifierProcess::TopologyClassifierProcess(
                                                 const std::string& fileNameOut,
                                                 const std::string& mode,
                                                 const double& threshold1,
                                                 const double& threshold2,
                                                 const bool& ascii,
                                                 const std::string& format,
                                                 const bool& verbose)
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
                   &TopologyClassifierProcess::classification< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &TopologyClassifierProcess::classification< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &TopologyClassifierProcess::classification< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &TopologyClassifierProcess::classification< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &TopologyClassifierProcess::classification< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &TopologyClassifierProcess::classification< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &TopologyClassifierProcess::classification< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &TopologyClassifierProcess::classification< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &TopologyClassifierProcess::classification< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &TopologyClassifierProcess::classification< double > );

}


const std::string& TopologyClassifierProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& TopologyClassifierProcess::getMode() const
{

  return _mode;

}


const double& TopologyClassifierProcess::getThreshold1() const
{

  return _threshold1;

}


const double& TopologyClassifierProcess::getThreshold2() const
{

  return _threshold2;

}


const bool& TopologyClassifierProcess::getAscii() const
{

  return _ascii;

}


const std::string& TopologyClassifierProcess::getFormat() const
{

  return _format;

}


const bool& TopologyClassifierProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void TopologyClassifierProcess::classification( gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    TopologyClassifierProcess&
      topologyClassifierProcess = static_cast< TopologyClassifierProcess& >( 
                                                                      process );

    // reading data
    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "classification : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                               topologyClassifierProcess.getMode(),
			       ( T )topologyClassifierProcess.getThreshold1(),
			       ( T )topologyClassifierProcess.getThreshold2() );
    
    gkg::Volume< uint8_t > volumeOut;
    volumeOut = gkg::TopologyClassifier::getInstance().getClassification(
                                                                volume,
                                                                *testFunction );

    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "writing '"
                << topologyClassifierProcess.getFileNameOut() 
                << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( 
                                     topologyClassifierProcess.getFileNameOut(),
                                     volumeOut,
                                     topologyClassifierProcess.getAscii(),
                                     topologyClassifierProcess.getFormat() );
    if ( topologyClassifierProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
    
    delete testFunction;

  }
  GKG_CATCH( "template < class T > "
             "void TopologyClassifierProcess::classification( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   TopologyClassifierCommand
//


gkg::TopologyClassifierCommand::TopologyClassifierCommand( int32_t argc,
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
  GKG_CATCH( "gkg::TopologyClassifierCommand::TopologyClassifierCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TopologyClassifierCommand::TopologyClassifierCommand(
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
  GKG_CATCH( "gkg::TopologyClassifierCommand::TopologyClassifierCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::TopologyClassifierCommand::TopologyClassifierCommand(
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
  GKG_CATCH( "gkg::TopologyClassifierCommand::TopologyClassifierCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TopologyClassifierCommand::~TopologyClassifierCommand()
{
}


std::string gkg::TopologyClassifierCommand::getStaticName()
{

  try
  {

    return "TopologyClassifier";

  }
  GKG_CATCH( "std::string gkg::TopologyClassifierCommand::getStaticName()" );

}


void gkg::TopologyClassifierCommand::parse()
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
                                  "Topology classification on container",
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
  GKG_CATCH_FUNCTOR( "void gkg::TopologyClassifierCommand::parse()" );

}


void gkg::TopologyClassifierCommand::execute( const std::string& fileNameIn,
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

      throw std::runtime_error( "invalid threshold mode" );

    }
         
    //
    // launching process
    //
    TopologyClassifierProcess topologyClassifierProcess( fileNameOut,
                                                         mode,
                                                         threshold1,
                                                         threshold2,
                                                         ascii,
                                                         format,
                                                         verbose );
    topologyClassifierProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::TopologyClassifierCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& mode, double threshold1, double threshold2, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( TopologyClassifierCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( mode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
