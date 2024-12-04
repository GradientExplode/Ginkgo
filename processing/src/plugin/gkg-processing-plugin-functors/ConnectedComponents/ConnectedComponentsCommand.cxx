#include <gkg-processing-plugin-functors/ConnectedComponents/ConnectedComponentsCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
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


class ConnectedComponentsProcess : public gkg::Process
{

  public:

    ConnectedComponentsProcess( const std::string& fileNameOut,
                                const int32_t& rank,
                                const std::string& outType,
                                const std::string& mode,
                                const double& threshold1,
                                const double& threshold2,
                                const double& background,
                                const uint32_t& minimumComponentSize,
                                const uint32_t& maximumComponentCount,
                                const bool& binary,
                                const int32_t& neighborhood,
                                const int32_t& stride,
                                const bool& ascii,
                                const std::string& format,
                                const bool& verbose );

    const std::string& getFileNameOut() const;
    const int32_t& getRank() const;
    const std::string& getOutType() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getBackground() const;
    const uint32_t& getMinimumComponentSize() const;
    const uint32_t& getMaximumComponentCount() const;
    const bool& getBinary() const;
    const int32_t& getNeighborhood() const;
    const int32_t& getStride() const;
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
    const int32_t& _rank;
    const std::string& _outType;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _background;
    const uint32_t& _minimumComponentSize;
    const uint32_t& _maximumComponentCount;
    const bool& _binary;
    const int32_t& _neighborhood;
    const int32_t& _stride;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


ConnectedComponentsProcess::ConnectedComponentsProcess(
                                            const std::string& fileNameOut,
                                            const int32_t& rank,
                                            const std::string& outType,
                                            const std::string& mode,
                                            const double& threshold1,
                                            const double& threshold2,
                                            const double& background,
                                            const uint32_t& minimumComponentSize,
                                            const uint32_t& maximumComponentCount,
                                            const bool& binary,
                                            const int32_t& neighborhood,
                                            const int32_t& stride,
                                            const bool& ascii,
                                            const std::string& format,
                                            const bool& verbose )
                           : gkg::Process(),
                             _fileNameOut( fileNameOut ),
                             _rank( rank ),
                             _outType( outType ),
                             _mode( mode ),
                             _threshold1( threshold1 ),
                             _threshold2( threshold2 ),
                             _background( background ),
                             _minimumComponentSize( minimumComponentSize ),
                             _maximumComponentCount( maximumComponentCount ),
                             _binary( binary ),
                             _neighborhood( neighborhood ),
                             _stride( stride ),
                             _ascii( ascii ),
                             _format( format ),
                             _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &ConnectedComponentsProcess::operate< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &ConnectedComponentsProcess::operate< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &ConnectedComponentsProcess::operate< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &ConnectedComponentsProcess::operate< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &ConnectedComponentsProcess::operate< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &ConnectedComponentsProcess::operate< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &ConnectedComponentsProcess::operate< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &ConnectedComponentsProcess::operate< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &ConnectedComponentsProcess::operate< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &ConnectedComponentsProcess::operate< double > );

}


const std::string& ConnectedComponentsProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const int32_t& ConnectedComponentsProcess::getRank() const
{

  return _rank;

}


const std::string& ConnectedComponentsProcess::getOutType() const
{

  return _outType;

}


const std::string& ConnectedComponentsProcess::getMode() const
{

  return _mode;

}


const double& ConnectedComponentsProcess::getThreshold1() const
{

  return _threshold1;

}


const double& ConnectedComponentsProcess::getThreshold2() const
{

  return _threshold2;

}


const double& ConnectedComponentsProcess::getBackground() const
{

  return _background;

}


const uint32_t& ConnectedComponentsProcess::getMinimumComponentSize() const
{

  return _minimumComponentSize;

}


const uint32_t& ConnectedComponentsProcess::getMaximumComponentCount() const
{

  return _maximumComponentCount;

}


const bool& ConnectedComponentsProcess::getBinary() const
{

  return _binary;

}


const int32_t& ConnectedComponentsProcess::getNeighborhood() const
{

  return _neighborhood;

}


const int32_t& ConnectedComponentsProcess::getStride() const
{

  return _stride;

}


const bool& ConnectedComponentsProcess::getAscii() const
{

  return _ascii;

}


const std::string& ConnectedComponentsProcess::getFormat() const
{

  return _format;

}


const bool& ConnectedComponentsProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void ConnectedComponentsProcess::operate( gkg::Process& process,
                                          const std::string& fileNameIn,
                                          const gkg::AnalyzedObject&,
                                          const std::string& )
{

  try
  {

    ConnectedComponentsProcess& connectedComponentsProcess =
      static_cast< ConnectedComponentsProcess& >( process );

    // reading data
    if ( connectedComponentsProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( connectedComponentsProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( connectedComponentsProcess.getRank() >= volumeIn.getSizeT() )
    {

      throw std::runtime_error( "invalid rank" );

    }

    if ( connectedComponentsProcess.getVerbose() )
    {

      std::cout << "searching connected components : " 
                << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 0;
    if ( connectedComponentsProcess.getMode() == "lt" )
    {

      testFunction = new gkg::LowerThanFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "le" )
    {

      testFunction = new gkg::LowerOrEqualToFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "eq" )
    {

      testFunction = new gkg::EqualToFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "di" )
    {

      testFunction = new gkg::DifferentFromFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "ge" )
    {

      testFunction = new gkg::GreaterOrEqualToFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "gt" )
    {

      testFunction = new gkg::GreaterThanFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1() );

    }
    else if ( connectedComponentsProcess.getMode() == "be" )
    {

      testFunction = new gkg::BetweenOrEqualToFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1(),
                              ( T )connectedComponentsProcess.getThreshold2() );

    }
    else if ( connectedComponentsProcess.getMode() == "bt" )
    {

      testFunction = new gkg::BetweenFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1(),
                              ( T )connectedComponentsProcess.getThreshold2() );

    }
    else if ( connectedComponentsProcess.getMode() == "oe" )
    {

      testFunction = new gkg::OutsideOrEqualToFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1(),
                              ( T )connectedComponentsProcess.getThreshold2() );

    }
    else if ( connectedComponentsProcess.getMode() == "ou" )
    {

      testFunction = new gkg::OutsideFunction< T >(
                              ( T )connectedComponentsProcess.getThreshold1(),
                              ( T )connectedComponentsProcess.getThreshold2() );

    }

    std::string outType = connectedComponentsProcess.getOutType();
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    gkg::Neighborhood3d::Type 
      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_26;
    if ( connectedComponentsProcess.getNeighborhood() == 6 )
    {

      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_6;

    }
    else if ( connectedComponentsProcess.getNeighborhood() == 18 )
    {

      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_18;

    }
    else if ( connectedComponentsProcess.getNeighborhood() == 26 )
    {

      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_26;

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;

      gkg::ConnectedComponents< T, uint8_t >
        connectedComponents( 
                 *testFunction,
                 ( uint8_t )connectedComponentsProcess.getBackground(),
                 ( size_t )connectedComponentsProcess.getMinimumComponentSize(),
                 ( size_t )connectedComponentsProcess.getMaximumComponentCount(),
                 connectedComponentsProcess.getBinary(),
                 neighborhoodType,
                 connectedComponentsProcess.getStride() );
      connectedComponents.getComponents(
                                      volumeIn,
                                      volumeOut,
                                      connectedComponentsProcess.getRank(),
                                      connectedComponentsProcess.getVerbose() );

      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << std::endl;

      }

      // writing data
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << connectedComponentsProcess.getFileNameOut()
                  << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write(
                                    connectedComponentsProcess.getFileNameOut(),
                                    volumeOut,
                                    connectedComponentsProcess.getAscii(),
                                    connectedComponentsProcess.getFormat() );
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;

      gkg::ConnectedComponents< T, int16_t >
        connectedComponents( 
                *testFunction,
                ( int16_t )connectedComponentsProcess.getBackground(),
                ( size_t )connectedComponentsProcess.getMinimumComponentSize(),
                ( size_t )connectedComponentsProcess.getMaximumComponentCount(),
                connectedComponentsProcess.getBinary(),
                neighborhoodType,
                connectedComponentsProcess.getStride() );
      connectedComponents.getComponents(
                                      volumeIn,
                                      volumeOut,
                                      connectedComponentsProcess.getRank(),
                                      connectedComponentsProcess.getVerbose() );

      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << std::endl;

      }

      // writing data
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << connectedComponentsProcess.getFileNameOut()
                  << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write(
                                    connectedComponentsProcess.getFileNameOut(),
                                    volumeOut,
                                    connectedComponentsProcess.getAscii(),
                                    connectedComponentsProcess.getFormat() );
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( outType ) )
    {

      gkg::Volume< int32_t > volumeOut;

      gkg::ConnectedComponents< T, int32_t >
        connectedComponents( 
                *testFunction,
                ( int32_t )connectedComponentsProcess.getBackground(),
                ( size_t )connectedComponentsProcess.getMinimumComponentSize(),
                ( size_t )connectedComponentsProcess.getMaximumComponentCount(),
                connectedComponentsProcess.getBinary(),
                neighborhoodType,
                connectedComponentsProcess.getStride() );
      connectedComponents.getComponents(
                                      volumeIn,
                                      volumeOut,
                                      connectedComponentsProcess.getRank(),
                                      connectedComponentsProcess.getVerbose() );

      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << std::endl;

      }

      // writing data
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << connectedComponentsProcess.getFileNameOut()
                  << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write(
                                    connectedComponentsProcess.getFileNameOut(),
                                    volumeOut,
                                    connectedComponentsProcess.getAscii(),
                                    connectedComponentsProcess.getFormat() );
      if ( connectedComponentsProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void ConnectedComponentsProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class ConnectedComponentsCommand
//


gkg::ConnectedComponentsCommand::ConnectedComponentsCommand( int32_t argc,
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
  GKG_CATCH( "gkg::ConnectedComponentsCommand::ConnectedComponentsCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::ConnectedComponentsCommand::ConnectedComponentsCommand(
                                		 const std::string& fileNameIn,
                                		 const std::string& fileNameOut,
                                		 int32_t rank,
                                		 const std::string& outType,
                                		 const std::string& mode,
                                		 double threshold1,
                                		 double threshold2,
                                		 double background,
                                		 uint32_t minimumComponentSize,
                                		 uint32_t maximumComponentCount,
                                		 bool binary,
                                		 int32_t neighborhood,
                                		 int32_t stride,
                                		 bool ascii,
                                		 const std::string& format,
                                		 bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameOut, rank, outType, mode, threshold1,
             threshold2, background, minimumComponentSize, 
             maximumComponentCount, binary, neighborhood, stride,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ConnectedComponentsCommand::ConnectedComponentsCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t rank, const std::string& outType, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, uint32_t minimumComponentSize, "
             "uint32_t maximumComponentCount, bool binary, "
             "int32_t neighborhood, int32_t stride, bool ascii, "
             "const std::string& format, bool verbose )" );

}


gkg::ConnectedComponentsCommand::ConnectedComponentsCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rank );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_INTEGER_PARAMETER( parameters, uint32_t, minimumComponentSize );
    DECLARE_INTEGER_PARAMETER( parameters, uint32_t, maximumComponentCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, binary );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, neighborhood );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stride );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, rank, outType, mode, threshold1,
             threshold2, background, minimumComponentSize, 
             maximumComponentCount, binary, neighborhood, stride,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ConnectedComponentsCommand::ConnectedComponentsCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ConnectedComponentsCommand::~ConnectedComponentsCommand()
{
}


std::string gkg::ConnectedComponentsCommand::getStaticName()
{

  try
  {

    return "ConnectedComponents";

  }
  GKG_CATCH( "std::string gkg::ConnectedComponentsCommand::getStaticName()" );

}


void gkg::ConnectedComponentsCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t rank = 0;
    std::string outType = "short";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double background = 0.0;
    uint32_t minimumComponentSize = 1;
    uint32_t maximumComponentCount = 0;
    bool binary = false;
    int32_t neighborhood = 26;
    int32_t stride = 1;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Connected components extractor",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-r",
                                 "Rank in input volume "
                                 "(default=0)",
                                 rank,
                                 true );
    application.addSingleOption( "-t",
                                 "Output type unsigned char/short/int "
                                 "(default=short)",
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
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-s",
                                 "Minimum component size (default=1)",
                                 minimumComponentSize,
                                 true );
    application.addSingleOption( "-c",
                                 "Maximum component count (default=infinity)",
                                 maximumComponentCount,
                                 true );
    application.addSingleOption( "-B",
                                 "Consider input as a binary volume",
                                 binary,
                                 true );
    application.addSingleOption( "-N",
                                 "Neighborhood (default=26)\n"
                                 "-  6 -> 6 neighbors\n"
                                 "- 18 -> 18 neighbors\n"
                                 "- 26 -> 26 neighbors\n",
                                 neighborhood,
                                 true );
    application.addSingleOption( "-stride",
                                 "Stride (default=1)",
                                 stride,
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

    execute( fileNameIn, fileNameOut, rank, outType, mode, threshold1,
             threshold2, background, minimumComponentSize, 
             maximumComponentCount, binary, neighborhood, stride,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::ConnectedComponentsCommand::parse()" );

}


void gkg::ConnectedComponentsCommand::execute( const std::string& fileNameIn,
                                	       const std::string& fileNameOut,
                                	       int32_t rank,
                                	       const std::string& outType,
                                	       const std::string& mode,
                                	       double threshold1,
                                	       double threshold2,
                                	       double background,
                                	       uint32_t minimumComponentSize,
                                	       uint32_t maximumComponentCount,
                                	       bool binary,
                                	       int32_t neighborhood,
                                	       int32_t stride,
                                	       bool ascii,
                                	       const std::string& format,
                                	       bool verbose )
{

  try
  {

    if ( !outType.empty() &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

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

    if ( ( neighborhood != 6 ) &&
         ( neighborhood != 18 ) &&
         ( neighborhood != 26 ) )
    {

      throw std::runtime_error( "invalid neighborhood" );

    }

    //
    // launching process
    //
    ConnectedComponentsProcess connectedComponentsProcess(
                                         		  fileNameOut,
                                         		  rank,
                                         		  outType,
                                         		  mode,
                                         		  threshold1,
                                         		  threshold2,
                                         		  background,
                                         		  minimumComponentSize,
                                         		  maximumComponentCount,
                                         		  binary,
                                         		  neighborhood,
                                         		  stride,
                                         		  ascii,
                                         		  format,
                                         		  verbose );

    connectedComponentsProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::ConnectedComponentsCommand::execute( "
             " const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t rank, const std::string& outType, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, uint32_t minimumComponentSize, "
             "uint32_t maximumComponentCount, bool binary, "
             "int32_t neighborhood, int32_t stride, bool ascii, "
             "const std::string& format, bool verbose)" );

}


RegisterCommandCreator(
                       ConnectedComponentsCommand,
                       DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                       DECLARE_INTEGER_PARAMETER_HELP( rank ) +
                       DECLARE_STRING_PARAMETER_HELP( outType ) +
                       DECLARE_STRING_PARAMETER_HELP( mode ) +
                       DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                       DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                       DECLARE_FLOATING_PARAMETER_HELP( background ) +
                       DECLARE_INTEGER_PARAMETER_HELP( minimumComponentSize ) +
                       DECLARE_INTEGER_PARAMETER_HELP( maximumComponentCount ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( binary ) +
                       DECLARE_INTEGER_PARAMETER_HELP( neighborhood ) +
                       DECLARE_INTEGER_PARAMETER_HELP( outType ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( stride ) +
                       DECLARE_STRING_PARAMETER_HELP( format ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
