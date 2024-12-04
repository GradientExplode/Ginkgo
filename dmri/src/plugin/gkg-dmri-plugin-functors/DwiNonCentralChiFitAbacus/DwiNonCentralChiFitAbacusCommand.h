#ifndef _gkg_dmri_plugin_functors_DwiNonCentralChiFitAbacus_DwiNonCentralChiFitAbacusCommand_h_
#define _gkg_dmri_plugin_functors_DwiNonCentralChiFitAbacus_DwiNonCentralChiFitAbacusCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiNonCentralChiFitAbacusCommand :
                           public Command,
                           public Creator2Arg< DwiNonCentralChiFitAbacusCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiNonCentralChiFitAbacusCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiNonCentralChiFitAbacusCommand( int32_t argc,
                                      char* argv[],
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    DwiNonCentralChiFitAbacusCommand( const std::string& outputName,
                                      const std::vector< double >& vs,
                                      const std::vector< double >& sigmas,
                                      double channelCount,
                                      int32_t gaussianCount,
                                      int32_t trialCount,
                                      double confidence,
                                      int32_t maximumIterationCount,
                                      double absoluteError,
                                      double relativeError,
                                      const std::string& type,
                                      bool verbose );
    DwiNonCentralChiFitAbacusCommand( const Dictionary& parameters );
    virtual ~DwiNonCentralChiFitAbacusCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiNonCentralChiFitAbacusCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiNonCentralChiFitAbacusCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& outputName,
                  const std::vector< double >& vs,
                  const std::vector< double >& sigmas,
                  double channelCount,
                  int32_t gaussianCount,
                  int32_t trialCount,
                  double confidence,
                  int32_t maximumIterationCount,
                  double absoluteError,
                  double relativeError,
                  const std::string& type,
                  bool verbose );

};


}


#endif
