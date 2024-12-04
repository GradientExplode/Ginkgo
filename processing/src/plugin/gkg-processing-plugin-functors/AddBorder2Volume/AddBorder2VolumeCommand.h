#ifndef _gkg_processing_plugin_functors_AddBorder2Volume_AddBorder2VolumeCommand_h_
#define _gkg_processing_plugin_functors_AddBorder2Volume_AddBorder2VolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AddBorder2VolumeCommand : public Command,
                                public Creator2Arg< AddBorder2VolumeCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< AddBorder2VolumeCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    AddBorder2VolumeCommand( int32_t argc,
                             char* argv[], 
                             bool loadPlugin = false,
                             bool removeFirst = true );
    AddBorder2VolumeCommand(
                         const std::string& fileNameIn,
                         const std::string& fileNameOut,
                         int32_t borderSizeX,
                         int32_t borderSizeY,
                         int32_t borderSizeZ,
                         int32_t borderSizeT,
                         const std::string& fileNameNativeToExtendedTransform3d,
                         const std::string& fileNameExtendedToNativeTransform3d,
                         double background,
                         bool ascii,
                         const std::string& format,
                         bool verbose );
    AddBorder2VolumeCommand( const Dictionary& parameters );
    virtual ~AddBorder2VolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AddBorder2VolumeCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AddBorder2VolumeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t borderSizeX,
                  int32_t borderSizeY,
                  int32_t borderSizeZ,
                  int32_t borderSizeT,
                  const std::string& fileNameNativeToExtendedTransform3d,
                  const std::string& fileNameExtendedToNativeTransform3d,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
