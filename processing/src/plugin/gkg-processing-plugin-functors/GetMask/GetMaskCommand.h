#ifndef _gkg_processing_plugin_functors_GetMask_GetMaskCommand_h_
#define _gkg_processing_plugin_functors_GetMask_GetMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GetMaskCommand : public Command,
                       public Creator2Arg< GetMaskCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< GetMaskCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    GetMaskCommand( int32_t argc,
                    char* argv[], 
                    bool loadPlugin = false,
                    bool removeFirst = true );
    GetMaskCommand( const std::string& fileNameIn,
                    const std::string& fileNameOut,
                    int32_t algorithmType,
                    const std::string& outType,
    		    float radius,
    		    double noisePosition,
    		    double noiseRatio,
    		    int32_t thrown,
    		    int32_t kept,
    		    double thresholdRatio,
    		    double foreground,
    		    double background,
    		    bool ascii,
    		    const std::string& format,
    		    bool verbose );
    GetMaskCommand( const Dictionary& parameters );
    virtual ~GetMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GetMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GetMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t algorithmType,
                  const std::string& outType,
    		  float radius,
    		  double noisePosition,
    		  double noiseRatio,
    		  int32_t thrown,
    		  int32_t kept,
    		  double thresholdRatio,
    		  double foreground,
    		  double background,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
