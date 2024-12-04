#ifndef _gkg_processing_plugin_functors_RgbIntensityMapper_RgbIntensityMapperCommand_h_
#define _gkg_processing_plugin_functors_RgbIntensityMapper_RgbIntensityMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class RgbIntensityMapperCommand : public Command,
                         public Creator2Arg< RgbIntensityMapperCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< RgbIntensityMapperCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    RgbIntensityMapperCommand( int32_t argc,
                      char* argv[], 
                      bool loadPlugin = false,
                      bool removeFirst = true );
    RgbIntensityMapperCommand( const std::string& fileNameIn,
                      const std::string& fileNameOut,
                      bool ascii,
                      const std::string& format,
                      bool verbose );
    RgbIntensityMapperCommand( const Dictionary& parameters );
    virtual ~RgbIntensityMapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RgbIntensityMapperCommand, 
                              Command, int32_t, char** >;
    friend class Creator1Arg< RgbIntensityMapperCommand,
                              Command, const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
