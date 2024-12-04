#ifndef _gkg_processing_plugin_functors_GetTransform3dFromVolumeHeader_GetTransform3dFromVolumeHeaderCommand_h_
#define _gkg_processing_plugin_functors_GetTransform3dFromVolumeHeader_GetTransform3dFromVolumeHeaderCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GetTransform3dFromVolumeHeaderCommand :
                      public Command,
                      public Creator2Arg< GetTransform3dFromVolumeHeaderCommand,
                                          Command,
                                          int32_t,
                                          char** >,
                      public Creator1Arg< GetTransform3dFromVolumeHeaderCommand,
                                          Command,
                                          const Dictionary& >
{

  public:

    GetTransform3dFromVolumeHeaderCommand( int32_t argc,
                                           char* argv[], 
                                           bool loadPlugin = false,
                                           bool removeFirst = true );
    GetTransform3dFromVolumeHeaderCommand( const std::string& fileNameIn,
                                           const std::string& fileNameOut,
                                           bool verbose );
    GetTransform3dFromVolumeHeaderCommand( const Dictionary& parameters );
    virtual ~GetTransform3dFromVolumeHeaderCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GetTransform3dFromVolumeHeaderCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GetTransform3dFromVolumeHeaderCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
