#ifndef _gkg_processing_plugin_functors_PhaseUnwrapper_PhaseUnwrapperCommand_h_
#define _gkg_processing_plugin_functors_PhaseUnwrapper_PhaseUnwrapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class PhaseUnwrapperCommand : public Command,
                              public Creator2Arg< PhaseUnwrapperCommand,
                                         	  Command,
                                         	  int32_t,
                                         	  char** >,
                              public Creator1Arg< PhaseUnwrapperCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    PhaseUnwrapperCommand( int32_t argc,
                  	   char* argv[], 
                  	   bool loadPlugin = false,
                  	   bool removeFirst = true );
    PhaseUnwrapperCommand( const std::string& fileNameIn,
    			   const std::string& fileNameOut,
    			   const std::string& fileNameMask,
    			   const std::string& fileNameQuality,
    			   double offset,
    			   double scale,
    			   const std::string& planeAxis,
    			   int32_t cycleCount,
    			   int32_t coarsestSize,
    			   int32_t preSmoothingIterationCount,
    			   int32_t postSmoothingIterationCount,
    			   bool useCongruence,
    			   float qualityThreshold,
    			   bool removeRampFlag,
    			   bool ascii,
    			   const std::string& format,
    			   bool applyMaskToOutput,
    			   bool removeBias,
    			   const std::string& fileNameBias,
    			   int32_t maximumFitOrder,
    			   int32_t maximumBiasOrder,
    			   int32_t zUnwrappingType,
    			   bool verbose );
    PhaseUnwrapperCommand( const Dictionary& parameters );
    virtual ~PhaseUnwrapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< PhaseUnwrapperCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< PhaseUnwrapperCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
    		  const std::string& fileNameOut,
    		  const std::string& fileNameMask,
    		  const std::string& fileNameQuality,
    		  double offset,
    		  double scale,
    		  const std::string& planeAxis,
    		  int32_t cycleCount,
    		  int32_t coarsestSize,
    		  int32_t preSmoothingIterationCount,
    		  int32_t postSmoothingIterationCount,
    		  bool useCongruence,
    		  float qualityThreshold,
    		  bool removeRampFlag,
    		  bool ascii,
    		  const std::string& format,
    		  bool applyMaskToOutput,
    		  bool removeBias,
    		  const std::string& fileNameBias,
    		  int32_t maximumFitOrder,
    		  int32_t maximumBiasOrder,
    		  int32_t zUnwrappingType,
    		  bool verbose );

};


}


#endif
