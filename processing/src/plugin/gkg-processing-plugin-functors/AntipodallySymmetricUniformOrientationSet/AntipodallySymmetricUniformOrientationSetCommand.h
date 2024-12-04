#ifndef _gkg_processing_plugin_functors_AntipodallySymmetricUniformOrientationSet_AntipodallySymmetricUniformOrientationSetCommand_h_
#define _gkg_processing_plugin_functors_AntipodallySymmetricUniformOrientationSet_AntipodallySymmetricUniformOrientationSetCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AntipodallySymmetricUniformOrientationSetCommand :
           public Command,
           public Creator2Arg< AntipodallySymmetricUniformOrientationSetCommand,
                               Command,
                               int32_t,
                               char** >,
           public Creator1Arg< AntipodallySymmetricUniformOrientationSetCommand,
                               Command,
                               const Dictionary& >
{

  public:

    AntipodallySymmetricUniformOrientationSetCommand( int32_t argc,
                  		  char* argv[], 
                  		  bool loadPlugin = false,
                  		  bool removeFirst = true );
    AntipodallySymmetricUniformOrientationSetCommand(
                                  int32_t orientationCount,
                                  std::string& fileNameOut,
                                  bool verbose );
    AntipodallySymmetricUniformOrientationSetCommand(
                                  const Dictionary& parameters );
    virtual ~AntipodallySymmetricUniformOrientationSetCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AntipodallySymmetricUniformOrientationSetCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AntipodallySymmetricUniformOrientationSetCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( int32_t orientationCount,
                  std::string& fileNameOut,
                  bool verbose );

};


}


#endif
