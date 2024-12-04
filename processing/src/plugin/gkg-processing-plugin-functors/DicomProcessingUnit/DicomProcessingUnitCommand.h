#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DicomProcessingUnitCommand_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DicomProcessingUnitCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DicomProcessingUnitCommand :
                                 public Command,
                                 public Creator2Arg< DicomProcessingUnitCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< DicomProcessingUnitCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DicomProcessingUnitCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    DicomProcessingUnitCommand( std::string& incomingDirectory,
                                const std::string& daemonConfigName,
                                int32_t port,
                                std::string& transferSyntax,
                                std::string& applicationTitle,
                                int32_t acseTimeout,
                                int32_t eosTimeout,
                                const std::string& processHost,
                                int32_t processPort,
                                bool killDPU,
                                bool verbose );
    DicomProcessingUnitCommand( const Dictionary& parameters );
    virtual ~DicomProcessingUnitCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DicomProcessingUnitCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DicomProcessingUnitCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( std::string& incomingDirectory,
                  const std::string& daemonConfigName,
                  int32_t port,
                  std::string& transferSyntax,
                  std::string& applicationTitle,
                  int32_t acseTimeout,
                  int32_t eosTimeout,
                  const std::string& processHost,
                  int32_t processPort,
                  bool killDPU,
                  bool verbose );

};


}


#endif
