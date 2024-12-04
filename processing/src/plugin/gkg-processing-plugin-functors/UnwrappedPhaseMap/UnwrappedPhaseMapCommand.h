#ifndef _gkg_processing_plugin_functors_UnwrappedPhaseMap_UnwrappedPhaseMapCommand_h_
#define _gkg_processing_plugin_functors_UnwrappedPhaseMap_UnwrappedPhaseMapCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class UnwrappedPhaseMapCommand : public Command,
                                 public Creator2Arg< UnwrappedPhaseMapCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< UnwrappedPhaseMapCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    UnwrappedPhaseMapCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    UnwrappedPhaseMapCommand(
                            const std::string& fileNameDoubleEcho,
    			    const std::string& fileNameMagnitude1stEcho,
    			    const std::string& fileNameMagnitude2ndEcho,
    			    const std::string& fileNameWrappedPhaseDifference,
    			    const std::string& fileNameMorphoFilledMask,
    			    float closingRadiusFactor,
    			    double noisePosition,
    			    double noiseRatio,
    			    int32_t thrown,
    			    int32_t kept,
    			    const std::string& fileNameQualityMap,
    			    const std::string& fileNameQualityMask,
    			    float qualityThreshold,
    			    const std::string& fileNameUnwrappedPhaseDifference,
    			    const std::string& fileNameRegularizedPhaseMap,
    			    int32_t degree,
    			    float preFilteringSigma,
    			    const std::string& regularizationMethod,
    			    bool removeRampFlag,
    			    bool zLineUnwrapping,
    			    bool verbose );
    UnwrappedPhaseMapCommand( const Dictionary& parameters );
    virtual ~UnwrappedPhaseMapCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< UnwrappedPhaseMapCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< UnwrappedPhaseMapCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDoubleEcho,
    		  const std::string& fileNameMagnitude1stEcho,
    		  const std::string& fileNameMagnitude2ndEcho,
    		  const std::string& fileNameWrappedPhaseDifference,
    		  const std::string& fileNameMorphoFilledMask,
    		  float closingRadiusFactor,
    		  double noisePosition,
    		  double noiseRatio,
    		  int32_t thrown,
    		  int32_t kept,
    		  const std::string& fileNameQualityMap,
    		  const std::string& fileNameQualityMask,
    		  float qualityThreshold,
    		  const std::string& fileNameUnwrappedPhaseDifference,
    		  const std::string& fileNameRegularizedPhaseMap,
    		  int32_t degree,
    		  float preFilteringSigma,
    		  const std::string& regularizationMethod,
    		  bool removeRampFlag,
    		  bool zLineUnwrapping,
    		  bool verbose );

};


}


#endif
