#ifndef _gkg_processing_plugin_functors_HistogramAnalyzer_HistogramAnalyzerCommand_h_
#define _gkg_processing_plugin_functors_HistogramAnalyzer_HistogramAnalyzerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class HistogramAnalyzerCommand : public Command,
                                 public Creator2Arg< HistogramAnalyzerCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< HistogramAnalyzerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    HistogramAnalyzerCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    HistogramAnalyzerCommand( const std::string& fileNameIn,
                              const std::string& fileNameOut,
                              const std::string& fileNameMask,
                              double kernelWidth,
                              bool normalize,
                              bool cumulated,
                              int32_t levelCount,
                              double wantedLowerBoundary,
                              double wantedUpperBoundary,
                              bool matPlotLib,
                              bool verbose );
    HistogramAnalyzerCommand( const Dictionary& parameters );
    virtual ~HistogramAnalyzerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< HistogramAnalyzerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< HistogramAnalyzerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameMask,
                  double kernelWidth,
                  bool normalize,
                  bool cumulated,
                  int32_t levelCount,
                  double wantedLowerBoundary,
                  double wantedUpperBoundary,
                  bool matPlotLib,
                  bool verbose );

};


}


#endif
