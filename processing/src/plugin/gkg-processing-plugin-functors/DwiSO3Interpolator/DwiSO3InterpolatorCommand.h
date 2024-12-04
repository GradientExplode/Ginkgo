#ifndef _gkg_processing_plugin_functors_DwiSO3Interpolator_DwiSO3InterpolatorCommand_h_
#define _gkg_processing_plugin_functors_DwiSO3Interpolator_DwiSO3InterpolatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiSO3InterpolatorCommand : public Command,
                                  public Creator2Arg< DwiSO3InterpolatorCommand,
                                        	      Command,
                                        	      int32_t,
                                        	      char** >,
                                  public Creator1Arg< DwiSO3InterpolatorCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DwiSO3InterpolatorCommand( int32_t argc,
                 	       char* argv[], 
                 	       bool loadPlugin = false,
                 	       bool removeFirst = true );
    DwiSO3InterpolatorCommand( const std::string& fileNameIn,
    			       const std::string& fileNameOut,
    			       int32_t outputOrientationCount,
    			       int32_t maximumNeighborCount,
    			       float sigma,
    			       bool ascii,
    			       const std::string& outputFormat,
    			       bool verbose );
    DwiSO3InterpolatorCommand( const Dictionary& parameters );
    virtual ~DwiSO3InterpolatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSO3InterpolatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSO3InterpolatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
    		  const std::string& fileNameOut,
    		  int32_t outputOrientationCount,
    		  int32_t maximumNeighborCount,
    		  float sigma,
    		  bool ascii,
    		  const std::string& outputFormat,
    		  bool verbose );

};


}


#endif
