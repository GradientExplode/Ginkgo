#ifndef _gkg_fmri_plugin_functors_BoldAnalysis_BoldAnalysisCommand_h_
#define _gkg_fmri_plugin_functors_BoldAnalysis_BoldAnalysisCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class BoldAnalysisCommand : public Command,
                            public Creator2Arg< BoldAnalysisCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< BoldAnalysisCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    BoldAnalysisCommand( int32_t argc, 
                         char* argv[], 
                         bool loadPlugin = false,
                         bool removeFirst = true );
    BoldAnalysisCommand( const std::string& fileNameBOLD,
                         const std::string& fileNameAverageBOLD,
                         const std::string& fileNameSmoothedBOLD,
                         const std::string& fileNameMask,
                         const std::string& fileNameParadigm,
                         const std::string& fileNameContrastEffects,
                         const std::string& fileNameDesignMatrix,
                         const std::string& fileNameEffects,
                         const std::string& fileNameEffectVariances,
                         const std::vector< std::string >& mapAndName,
                         double startingTime,
                         double repetitionTime,
                         int32_t dummyScanCount,
                         int32_t driftPolynomOrder,
                         float smoothingFWHM,
                         const std::string& smoothingAxis,
                         const std::string& fileNameHRF,
                         double hrfDelay, 
                         double hrfDelayUndershoot, 
                         double hrfDispersion, 
                         double hrfDispersionUndershoot,
                         double hrfRatio, 
                         bool ascii,
                         const std::string& volumeFormat,
                         bool verbose );
    BoldAnalysisCommand( const Dictionary& parameters );
    virtual ~BoldAnalysisCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BoldAnalysisCommand, Command, int32_t, char** >;
    friend class Creator1Arg< BoldAnalysisCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBOLD,
                  const std::string& fileNameAverageBOLD,
                  const std::string& fileNameSmoothedBOLD,
                  const std::string& fileNameMask,
                  const std::string& fileNameParadigm,
                  const std::string& fileNameContrastEffects,
                  const std::string& fileNameDesignMatrix,
                  const std::string& fileNameEffects,
                  const std::string& fileNameEffectVariances,
                  const std::vector< std::string >& mapAndName,
                  double startingTime,
                  double repetitionTime,
                  int32_t dummyScanCount,
                  int32_t driftPolynomOrder,
                  float smoothingFWHM,
                  const std::string& smoothingAxis,
                  const std::string& fileNameHRF,
                  double hrfDelay, 
                  double hrfDelayUndershoot, 
                  double hrfDispersion, 
                  double hrfDispersionUndershoot,
                  double hrfRatio, 
                  bool ascii,
                  const std::string& volumeFormat,
                  bool verbose );

};


}


#endif
