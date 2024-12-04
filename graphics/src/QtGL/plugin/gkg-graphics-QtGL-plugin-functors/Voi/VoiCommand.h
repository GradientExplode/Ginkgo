#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_VoiCommand_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_VoiCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


class QApplication;


namespace gkg
{


class VoiCommand : public Command,
                   public Creator2Arg< VoiCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< VoiCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    VoiCommand( int32_t argc,
                char* argv[], 
                bool loadPlugin = false,
                bool removeFirst = true );
    VoiCommand( const std::string& anatName,
                const std::string& fusionName,
                const std::string& labelName,
                const std::string& transfoName );
    VoiCommand( const Dictionary& parameters );
    virtual ~VoiCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VoiCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VoiCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& anatName,
                  const std::string& fusionName,
                  const std::string& labelName,
                  const std::string& transfoName );

  private:

    QApplication* _qtApplication;

};


}


#endif
