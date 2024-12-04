#ifndef _gkg_processing_plugin_functors_ConnectedComponents_ConnectedComponentsCommand_h_
#define _gkg_processing_plugin_functors_ConnectedComponents_ConnectedComponentsCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class ConnectedComponentsCommand :
                                 public Command,
                                 public Creator2Arg< ConnectedComponentsCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< ConnectedComponentsCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    ConnectedComponentsCommand( int32_t argc,
                                char* argv[], 
                                bool loadPlugin = false,
                                bool removeFirst = true );
    ConnectedComponentsCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                int32_t rank,
                                const std::string& outType,
                                const std::string& mode,
                                double threshold1,
                                double threshold2,
                                double background,
                                uint32_t minimumComponentSize,
                                uint32_t maximumComponentCount,
                                bool binary,
                                int32_t neighborhood,
                                int32_t stride,
                                bool ascii,
                                const std::string& format,
                                bool verbose );
    ConnectedComponentsCommand( const Dictionary& parameters );
    virtual ~ConnectedComponentsCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< ConnectedComponentsCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< ConnectedComponentsCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t rank,
                  const std::string& outType,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double background,
                  uint32_t minimumComponentSize,
                  uint32_t maximumComponentCount,
                  bool binary,
                  int32_t neighborhood,
                  int32_t stride,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
