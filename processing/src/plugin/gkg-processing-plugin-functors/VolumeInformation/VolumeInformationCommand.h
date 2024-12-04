#ifndef _gkg_processing_plugin_functors_VolumeInformation_VolumeInformationCommand_h_
#define _gkg_processing_plugin_functors_VolumeInformation_VolumeInformationCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VolumeInformationCommand : public Command,
                                 public Creator2Arg< VolumeInformationCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< VolumeInformationCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    VolumeInformationCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    VolumeInformationCommand( const std::string& fileNameIn,
                              const std::string& information, 
                              bool verbose );
    VolumeInformationCommand( const Dictionary& parameters );
    virtual ~VolumeInformationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeInformationCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VolumeInformationCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& information, 
                  bool verbose );

};


}


#endif
