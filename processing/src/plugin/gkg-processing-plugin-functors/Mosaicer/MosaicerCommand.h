#ifndef _gkg_processing_plugin_functors_Mosaicer_MosaicerCommand_h_
#define _gkg_processing_plugin_functors_Mosaicer_MosaicerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MosaicerCommand : public Command,
                        public Creator2Arg< MosaicerCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< MosaicerCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    MosaicerCommand( int32_t argc,
                     char* argv[], 
                     bool loadPlugin = false,
                     bool removeFirst = true );
    MosaicerCommand( const std::string& fileNameIn,
                     const std::string& fileNameOut,
                     const std::string& outputFormat,
                     bool writeSiemensTags,
                     bool verbose );
    MosaicerCommand( const Dictionary& parameters );
    virtual ~MosaicerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MosaicerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MosaicerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool writeSiemensTags,
                  bool verbose );

};


}


#endif
