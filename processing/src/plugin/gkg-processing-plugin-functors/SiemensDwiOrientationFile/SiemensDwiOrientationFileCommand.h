#ifndef _gkg_processing_plugin_functors_SiemensDwiOrientationFile_SiemensDwiOrientationFileCommand_h_
#define _gkg_processing_plugin_functors_SiemensDwiOrientationFile_SiemensDwiOrientationFileCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class SiemensDwiOrientationFileCommand :
                           public Command,
                           public Creator2Arg< SiemensDwiOrientationFileCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< SiemensDwiOrientationFileCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    SiemensDwiOrientationFileCommand( int32_t argc,
                  		      char* argv[], 
                  		      bool loadPlugin = false,
                  		      bool removeFirst = true );
    SiemensDwiOrientationFileCommand( 
                                  const std::vector< int32_t >& directionCounts,
                                  const std::string& fileNameIn,
                                  const std::string& fileNameOut,
                                  bool normalize,
                                  bool verbose );
    SiemensDwiOrientationFileCommand( const Dictionary& parameters );
    virtual ~SiemensDwiOrientationFileCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SiemensDwiOrientationFileCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SiemensDwiOrientationFileCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< int32_t >& directionCounts,
                  const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool normalize,
                  bool verbose );

};


}


#endif
