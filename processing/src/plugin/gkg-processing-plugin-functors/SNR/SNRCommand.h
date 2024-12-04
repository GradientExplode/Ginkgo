#ifndef _gkg_processing_plugin_functors_SNR_SNRCommand_h_
#define _gkg_processing_plugin_functors_SNR_SNRCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class SNRCommand : public Command,
                   public Creator2Arg< SNRCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< SNRCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    SNRCommand( int32_t argc,
                char* argv[], 
                bool loadPlugin = false,
                bool removeFirst = true );
    SNRCommand( const std::string& fileNameIn,
                const std::string& fileNameMask,
                std::vector< int32_t >& signalBoundingBox,
                std::vector< int32_t >& noiseBoundingBox,
                int32_t time,
                const std::string& fileNameOut,
                bool verbose );
    SNRCommand( const Dictionary& parameters );
    virtual ~SNRCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SNRCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SNRCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameMask,
                  std::vector< int32_t >& signalBoundingBox,
                  std::vector< int32_t >& noiseBoundingBox,
                  int32_t time,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
