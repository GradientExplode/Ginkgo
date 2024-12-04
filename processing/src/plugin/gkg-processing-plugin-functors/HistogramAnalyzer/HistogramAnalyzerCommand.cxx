#include <gkg-processing-plugin-functors/HistogramAnalyzer/HistogramAnalyzerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


#define MAXIMUM_LEVEL_COUNT 2000000000


//
//  HistogramAnalyzerProcess
//


class HistogramAnalyzerProcess : public gkg::Process
{

  public:

    HistogramAnalyzerProcess( const std::string& fileNameMask,
                              const std::string& fileNameOut,
                              const double& kernelWidth,
                              const bool& normalize,
                              const bool& cumulated,
                              const int32_t& levelCount,
                              const double& wantedLowerBoundary,
                              const double& wantedUpperBoundary,
                              const bool& matPlotLib,
                              const bool& verbose );

    const std::string& getFileNameMask() const;
    const std::string& getFileNameOut() const;
    const double& getKernelWidth() const;
    const bool& getNormalize() const;
    const bool& getCumulated() const;
    const int32_t& getLevelCount() const;
    const double& getWantedLowerBoundary() const;
    const double& getWantedUpperBoundary() const;
    const bool& getMatPlotLib() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void analyze( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameMask;
    const std::string& _fileNameOut;
    const double& _kernelWidth;
    const bool& _normalize;
    const bool& _cumulated;
    const int32_t& _levelCount;
    const double& _wantedLowerBoundary;
    const double& _wantedUpperBoundary;
    const bool& _matPlotLib;
    const bool& _verbose;

};


HistogramAnalyzerProcess::HistogramAnalyzerProcess( 
                                              const std::string& fileNameMask,
                                              const std::string& fileNameOut,
                                              const double& kernelWidth,
                                              const bool& normalize,
                                              const bool& cumulated,
                                              const int32_t& levelCount,
                                              const double& wantedLowerBoundary,
                                              const double& wantedUpperBoundary,
                                              const bool& matPlotLib,
                                              const bool& verbose )
                         : gkg::Process( "Volume" ),
                           _fileNameMask( fileNameMask ),
                           _fileNameOut( fileNameOut ),
                           _kernelWidth( kernelWidth ),
                           _normalize( normalize ),
                           _cumulated( cumulated ),
                           _levelCount( levelCount ),
                           _wantedLowerBoundary( wantedLowerBoundary ),
                           _wantedUpperBoundary( wantedUpperBoundary ),
                           _matPlotLib( matPlotLib ),
                           _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &HistogramAnalyzerProcess::analyze< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &HistogramAnalyzerProcess::analyze< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &HistogramAnalyzerProcess::analyze< double > );

}


const std::string& HistogramAnalyzerProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::string& HistogramAnalyzerProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const double& HistogramAnalyzerProcess::getKernelWidth() const
{

  return _kernelWidth;

}


const bool& HistogramAnalyzerProcess::getNormalize() const
{

  return _normalize;

}


const bool& HistogramAnalyzerProcess::getCumulated() const
{

  return _cumulated;

}


const int32_t& HistogramAnalyzerProcess::getLevelCount() const
{

  return _levelCount;

}


const double& HistogramAnalyzerProcess::getWantedLowerBoundary() const
{

  return _wantedLowerBoundary;

}


const double& HistogramAnalyzerProcess::getWantedUpperBoundary() const
{

  return _wantedUpperBoundary;

}


const bool& HistogramAnalyzerProcess::getMatPlotLib() const
{

  return _matPlotLib;

}


const bool& HistogramAnalyzerProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void HistogramAnalyzerProcess::analyze( gkg::Process& process,
                                        const std::string& fileNameIn,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    T lowerBoundary, upperBoundary;
    HistogramAnalyzerProcess& 
      histogramAnalyzerProcess = static_cast< HistogramAnalyzerProcess& >( 
                                                                      process );

    // reading data
    if ( histogramAnalyzerProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }

    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );

    if ( histogramAnalyzerProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // reading data
    gkg::Volume< int16_t > mask;
    if ( !histogramAnalyzerProcess.getFileNameMask().empty() )
    {

      if ( histogramAnalyzerProcess.getVerbose() )
      {

        std::cout << "reading '" << histogramAnalyzerProcess.getFileNameMask()
                  << "' : " << std::flush;

      }

      gkg::Reader::getInstance().read(
                                  histogramAnalyzerProcess.getFileNameMask(),
                                  mask );

      if ( histogramAnalyzerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    if ( histogramAnalyzerProcess.getFileNameMask().empty() )
    {

      gkg::HistogramAnalyzer::getInstance().getBoundaries( volume,
                                                           lowerBoundary,
                                                           upperBoundary );

    }
    else
    {

      gkg::HistogramAnalyzer::getInstance().getBoundaries( volume,
                                                           mask,
                                                           lowerBoundary,
                                                           upperBoundary );

    }

    T usedLowerBoundary = std::max( lowerBoundary,
                                    ( T )histogramAnalyzerProcess.
                                                     getWantedLowerBoundary() );
    T usedUpperBoundary = std::min( upperBoundary,
                                    ( T )histogramAnalyzerProcess.
                                                     getWantedUpperBoundary() );
    if ( histogramAnalyzerProcess.getVerbose() )
    {

      std::cout << "data lower boundary : " << lowerBoundary << std::endl;
      std::cout << "data upper boundary : " << upperBoundary << std::endl;
      std::cout << "wanted lower boundary : "
                << histogramAnalyzerProcess.getWantedLowerBoundary()
                << std::endl;
      std::cout << "wanted upper boundary : "
                << histogramAnalyzerProcess.getWantedUpperBoundary()
                << std::endl;
      std::cout << "used lower boundary : " << usedLowerBoundary
                << std::endl;
      std::cout << "used upper boundary : " << usedUpperBoundary
                << std::endl;

    }


    std::vector< double > histogram;
    if ( histogramAnalyzerProcess.getLevelCount() == 0 )
    {

      int64_t levelCount = ( int64_t )( ( double )usedUpperBoundary -
                                        ( double )usedLowerBoundary + 1 );

      if ( ( levelCount < 0 ) || ( levelCount > MAXIMUM_LEVEL_COUNT ) )
      {

        throw std::runtime_error(
          "lower to upper boundary range may be too big: please choose "
          "a manual level count" ); 

      }
      histogram.resize( levelCount );

    }
    else
    {

      histogram.resize( histogramAnalyzerProcess.getLevelCount() );

    }

    if ( histogramAnalyzerProcess.getFileNameMask().empty() )
    {

      if ( histogramAnalyzerProcess.getCumulated() )
      {

        gkg::HistogramAnalyzer::getInstance().getCumulatedHistogram( 
                                      volume,
                                      histogram,
                                      usedLowerBoundary,
                                      usedUpperBoundary,
                                      histogramAnalyzerProcess.getKernelWidth(),
                                      histogramAnalyzerProcess.getNormalize() );

      }
      else
      {

        gkg::HistogramAnalyzer::getInstance().getHistogram( 
                                      volume,
                                      histogram,
                                      usedLowerBoundary,
                                      usedUpperBoundary,
                                      histogramAnalyzerProcess.getKernelWidth(),
                                      histogramAnalyzerProcess.getNormalize() );

      }

    }
    else
    {

      if ( histogramAnalyzerProcess.getCumulated() )
      {

        gkg::HistogramAnalyzer::getInstance().getCumulatedHistogram( 
                                      volume,
                                      mask,
                                      histogram,
                                      usedLowerBoundary,
                                      usedUpperBoundary,
                                      histogramAnalyzerProcess.getKernelWidth(),
                                      histogramAnalyzerProcess.getNormalize() );

      }
      else
      {

        gkg::HistogramAnalyzer::getInstance().getHistogram( 
                                      volume,
                                      mask,
                                      histogram,
                                      usedLowerBoundary,
                                      usedUpperBoundary,
                                      histogramAnalyzerProcess.getKernelWidth(),
                                      histogramAnalyzerProcess.getNormalize() );

      }

    }

    if ( !histogramAnalyzerProcess.getFileNameOut().empty() )
    {

      std::string fileNameOut = histogramAnalyzerProcess.getFileNameOut();
      std::ofstream os( fileNameOut.c_str() );

      double currentBinValue = ( double )usedLowerBoundary;
      double incrementValue = ( ( double )usedUpperBoundary - 
                                ( double )usedLowerBoundary ) /
                              ( ( double )histogram.size() - 1 );

      if ( histogramAnalyzerProcess.getMatPlotLib() )
      {

        os << "xs = [";

        std::vector< double >::iterator
          h = histogram.begin(),
          he = histogram.end();
        -- he;

        while ( h != he )
        {

          os << currentBinValue << ", ";
          currentBinValue += incrementValue;
          ++ h;

        }
        os << currentBinValue << " ]" << std::endl;

        os << "hs = [";
        h = histogram.begin();
        while ( h != he )
        {

          os << *h << ", ";
          currentBinValue += incrementValue;
          ++ h;

        }
        os << *h << " ]" << std::endl;


      }
      else
      {


        std::vector< double >::iterator
          h = histogram.begin(),
          he = histogram.end();

        while ( h != he )
        {

          os << currentBinValue << "\t" << *h << "\t" << std::endl;
          currentBinValue += incrementValue;
          ++ h;

        }

      }
      os.close();

    }
    else
    {

      double currentBinValue = ( double )usedLowerBoundary;
      double incrementValue = ( ( double )usedUpperBoundary - 
                                ( double )usedLowerBoundary ) /
                              ( ( double )histogram.size() - 1 );

      if ( histogramAnalyzerProcess.getMatPlotLib() )
      {

        std::cout << "xs = [";

        std::vector< double >::iterator
          h = histogram.begin(),
          he = histogram.end();
        -- he;

        while ( h != he )
        {

          std::cout << currentBinValue << ", ";
          currentBinValue += incrementValue;
          ++ h;

        }
        std::cout << currentBinValue << " ]" << std::endl;

        std::cout << "hs = [";
        h = histogram.begin();
        while ( h != he )
        {

          std::cout << *h << ", ";
          currentBinValue += incrementValue;
          ++ h;

        }
        std::cout << *h << " ]" << std::endl;


      }
      else
      {


        std::vector< double >::iterator
          h = histogram.begin(),
          he = histogram.end();

        while ( h != he )
        {

          std::cout << currentBinValue << "\t" << *h << "\t" << std::endl;
          currentBinValue += incrementValue;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void HistogramAnalyzerProcess::analyze( "
             "gkg::Process&, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   HistogramAnalyzerCommand
//


gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand( int32_t argc,
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
  GKG_CATCH( "gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand(
                              			const std::string& fileNameIn,
                              			const std::string& fileNameOut,
                              			const std::string& fileNameMask,
                              			double kernelWidth,
                              			bool normalize,
                              			bool cumulated,
                              			int32_t levelCount,
                                                double wantedLowerBoundary,
                                                double wantedUpperBoundary,
                              			bool matPlotLib,
                              			bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, fileNameMask, kernelWidth, normalize,
             cumulated, levelCount, wantedLowerBoundary, wantedUpperBoundary,
             matPlotLib, verbose );

  }
  GKG_CATCH( "gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& fileNameMask, double kernelWidth, "
             "bool normalize, bool cumulated, int32_t levelCount, "
             "bool matPlotLib, bool verbose )" );

}


gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_FLOATING_PARAMETER( parameters, double, kernelWidth );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, normalize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, cumulated );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, levelCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, wantedLowerBoundary );
    DECLARE_FLOATING_PARAMETER( parameters, double, wantedUpperBoundary );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, matPlotLib );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, fileNameMask, kernelWidth, normalize,
             cumulated, levelCount, wantedLowerBoundary, wantedUpperBoundary,
             matPlotLib, verbose );

  }
  GKG_CATCH( "gkg::HistogramAnalyzerCommand::HistogramAnalyzerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::HistogramAnalyzerCommand::~HistogramAnalyzerCommand()
{
}


std::string gkg::HistogramAnalyzerCommand::getStaticName()
{

  try
  {

    return "HistogramAnalyzer";

  }
  GKG_CATCH( "std::string gkg::HistogramAnalyzerCommand::getStaticName()" );

}


void gkg::HistogramAnalyzerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string fileNameMask;
    double kernelWidth = -1.0;
    bool normalize = false;
    bool cumulated = false;
    int32_t levelCount = 0;
    double wantedLowerBoundary = -1e38;
    double wantedUpperBoundary = +1e38;
    bool matPlotLib = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Histogram analysis of a container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-m",
                                 "Mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-o",
                                 "Output histogram file name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-w",
                                 "Parzen' kernel width (default=-1.0)",
                                 kernelWidth,
                                 true );
    application.addSingleOption( "-n",
                                 "Normalizing pdf",
                                 normalize,
                                 true );
    application.addSingleOption( "-cumulated",
                                 "Cumulated histogram",
                                 cumulated,
                                 true );
    application.addSingleOption( "-levelCount",
                                 "Histogram level count",
                                 levelCount,
                                 true );
    application.addSingleOption( "-wantedLowerBoundary",
                                 "Wanted lower boundary (default=-1e38)",
                                 wantedLowerBoundary,
                                 true );
    application.addSingleOption( "-wantedUpperBoundary",
                                 "Wanted upper boundary (default=+1e38)",
                                 wantedUpperBoundary,
                                 true );
    application.addSingleOption( "-matPlotLib",
                                 "Save in MatplotLib python format "
                                 "(default=false)",
                                 matPlotLib,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, fileNameMask, kernelWidth, normalize,
             cumulated, levelCount, wantedLowerBoundary, wantedUpperBoundary,
             matPlotLib, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::HistogramAnalyzerCommand::parse()" );

}


void gkg::HistogramAnalyzerCommand::execute( const std::string& fileNameIn,
                              		     const std::string& fileNameOut,
                              		     const std::string& fileNameMask,
                              		     double kernelWidth,
                              		     bool normalize,
                              		     bool cumulated,
                              		     int32_t levelCount,
                                             double wantedLowerBoundary,
                                             double wantedUpperBoundary,
                              		     bool matPlotLib,
                              		     bool verbose )
{

  try
  {

    //
    // sanity check(s)
    //
    if ( levelCount < 0 )
    {

      throw std::runtime_error( "level count must be positive" );

    }

    //
    // launching sub-volume process
    //
    HistogramAnalyzerProcess histogramAnalyzerProcess( fileNameMask,
                                                       fileNameOut,
                                                       kernelWidth,
                                                       normalize,
                                                       cumulated,
                                                       levelCount,
                                                       wantedLowerBoundary,
                                                       wantedUpperBoundary,
                                                       matPlotLib,
                                                       verbose );
    histogramAnalyzerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::HistogramAnalyzerCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& fileNameMask, double kernelWidth, "
             "bool normalize, bool cumulated, int32_t levelCount, "
             "bool matPlotLib, bool verbose )" );

}


RegisterCommandCreator( HistogramAnalyzerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_FLOATING_PARAMETER_HELP( kernelWidth ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( normalize ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( cumulated ) +
    			DECLARE_INTEGER_PARAMETER_HELP( levelCount ) +
    			DECLARE_FLOATING_PARAMETER_HELP( wantedLowerBoundary ) +
    			DECLARE_FLOATING_PARAMETER_HELP( wantedUpperBoundary ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( matPlotLib ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
