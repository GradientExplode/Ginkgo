#ifndef _gkg_processing_plugin_functors_AddInformationToVolumeHeader_AddInformationToVolumeHeaderCommand_h_
#define _gkg_processing_plugin_functors_AddInformationToVolumeHeader_AddInformationToVolumeHeaderCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AddInformationToVolumeHeaderCommand :
                        public Command,
                        public Creator2Arg< AddInformationToVolumeHeaderCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< AddInformationToVolumeHeaderCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    AddInformationToVolumeHeaderCommand( int32_t argc,
                                         char* argv[], 
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    AddInformationToVolumeHeaderCommand( const std::string& fileNameIn,
                                         const std::string& hanFileName,
                                         const std::string& acPcIhFileName,
                                         const std::string& brainMaskFileName,
                                         const std::string& fileNameOut,
                                         bool ascii,
                                         const std::string& format,
                                         bool verbose );
    AddInformationToVolumeHeaderCommand( const Dictionary& parameters );
    virtual ~AddInformationToVolumeHeaderCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AddInformationToVolumeHeaderCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AddInformationToVolumeHeaderCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& hanFileName,
                  const std::string& acPcIhFileName,
                  const std::string& brainMaskFileName,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
