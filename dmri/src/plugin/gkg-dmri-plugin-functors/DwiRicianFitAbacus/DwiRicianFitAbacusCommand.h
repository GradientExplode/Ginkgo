#ifndef _gkg_dmri_plugin_functors_DwiRicianFitAbacus_DwiRicianFitAbacusCommand_h_
#define _gkg_dmri_plugin_functors_DwiRicianFitAbacus_DwiRicianFitAbacusCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiRicianFitAbacusCommand : public Command,
                                  public Creator2Arg< DwiRicianFitAbacusCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< DwiRicianFitAbacusCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DwiRicianFitAbacusCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    DwiRicianFitAbacusCommand( const std::string& outputName,
                               const std::vector< double >& vs,
                               const std::vector< double >& sigmas,
                               int32_t gaussianCount,
                               int32_t trialCount,
                               double confidence,
                               int32_t maximumIterationCount,
                               double absoluteError,
                               double relativeError,
                               const std::string& type,
                               bool verbose );
    DwiRicianFitAbacusCommand( const Dictionary& parameters );
    virtual ~DwiRicianFitAbacusCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiRicianFitAbacusCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiRicianFitAbacusCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& outputName,
                  const std::vector< double >& vs,
                  const std::vector< double >& sigmas,
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
