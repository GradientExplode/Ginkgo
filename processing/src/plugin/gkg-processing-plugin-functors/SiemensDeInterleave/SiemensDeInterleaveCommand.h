#ifndef _gkg_processing_plugin_functors_SiemensDeInterleave_SiemensDeInterleaveCommand_h_
#define _gkg_processing_plugin_functors_SiemensDeInterleave_SiemensDeInterleaveCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SiemensDeInterleaveCommand :
                                 public Command,
                                 public Creator2Arg< SiemensDeInterleaveCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< SiemensDeInterleaveCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    SiemensDeInterleaveCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    SiemensDeInterleaveCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                const std::string& outputFormat,
                                bool verbose );
    SiemensDeInterleaveCommand( const Dictionary& parameters );
    virtual ~SiemensDeInterleaveCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SiemensDeInterleaveCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SiemensDeInterleaveCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
