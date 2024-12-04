#ifndef _gkg_processing_plugin_functors_DicomStorageUnit_DicomStorageUnitCommand_h_
#define _gkg_processing_plugin_functors_DicomStorageUnit_DicomStorageUnitCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DicomStorageUnitCommand : public Command,
                                public Creator2Arg< DicomStorageUnitCommand,
                                         	    Command,
                                         	    int32_t,
                                         	    char** >,
                                public Creator1Arg< DicomStorageUnitCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DicomStorageUnitCommand( int32_t argc,
                  	     char* argv[], 
                  	     bool loadPlugin = false,
                  	     bool removeFirst = true );
    DicomStorageUnitCommand( std::string& incomingDirectory,
    			     const std::string& daemonConfigName,
    			     int32_t port,
    			     std::string& transferSyntax,
    			     std::string& applicationTitle,
    			     int32_t acseTimeout,
    			     int32_t eosTimeout,
    			     bool verbose );
    DicomStorageUnitCommand( const Dictionary& parameters );
    virtual ~DicomStorageUnitCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DicomStorageUnitCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DicomStorageUnitCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( std::string& incomingDirectory,
    		  const std::string& daemonConfigName,
    		  int32_t port,
    		  std::string& transferSyntax,
    		  std::string& applicationTitle,
    		  int32_t acseTimeout,
    		  int32_t eosTimeout,
    		  bool verbose );

};


}


#endif
