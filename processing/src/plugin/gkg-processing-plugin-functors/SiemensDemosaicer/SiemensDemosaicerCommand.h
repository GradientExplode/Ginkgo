#ifndef _gkg_processing_plugin_functors_SiemensDemosaicer_SiemensDemosaicerCommand_h_
#define _gkg_processing_plugin_functors_SiemensDemosaicer_SiemensDemosaicerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SiemensDemosaicerCommand : public Command,
                                 public Creator2Arg< SiemensDemosaicerCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< SiemensDemosaicerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    SiemensDemosaicerCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    SiemensDemosaicerCommand( const std::string& fileNameIn,
                              const std::string& fileNameOut,
                              const std::string& outputFormat,
                              bool verbose );
    SiemensDemosaicerCommand( const Dictionary& parameters );
    virtual ~SiemensDemosaicerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SiemensDemosaicerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SiemensDemosaicerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
