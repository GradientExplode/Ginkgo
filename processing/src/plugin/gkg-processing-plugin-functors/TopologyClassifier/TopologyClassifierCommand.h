#ifndef _gkg_processing_plugin_functors_TopologyClassifier_TopologyClassifierCommand_h_
#define _gkg_processing_plugin_functors_TopologyClassifier_TopologyClassifierCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class TopologyClassifierCommand : public Command,
                                  public Creator2Arg< TopologyClassifierCommand,
                                         	      Command,
                                         	      int32_t,
                                         	      char** >,
                                  public Creator1Arg< TopologyClassifierCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    TopologyClassifierCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    TopologyClassifierCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               const std::string& mode,
                               double threshold1,
                               double threshold2,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    TopologyClassifierCommand( const Dictionary& parameters );
    virtual ~TopologyClassifierCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TopologyClassifierCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TopologyClassifierCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
