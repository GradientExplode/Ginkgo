#ifndef _gkg_processing_plugin_functors_VolumeBook_VolumeBookCommand_h_
#define _gkg_processing_plugin_functors_VolumeBook_VolumeBookCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class VolumeBookCommand : public Command,
                          public Creator2Arg< VolumeBookCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< VolumeBookCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    VolumeBookCommand( int32_t argc,
                       char* argv[], 
                       bool loadPlugin = false,
                       bool removeFirst = true );
    VolumeBookCommand( const std::string& fileNameIn,
                       const std::string& fileNameOut,
                       const std::vector< double >& greyLevelRange,
                       const std::vector< double >& pictureSizes,
                       int32_t smoothingFactor,
                       const std::string& title,
                       const std::string& author,
                       const std::string& date,
                       bool verbose );
    VolumeBookCommand( const Dictionary& parameters );
    virtual ~VolumeBookCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeBookCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VolumeBookCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::vector< double >& greyLevelRange,
                  const std::vector< double >& pictureSizes,
                  int32_t smoothingFactor,
                  const std::string& title,
                  const std::string& author,
                  const std::string& date,
                  bool verbose );

};


}


#endif
