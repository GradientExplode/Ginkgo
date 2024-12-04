#ifndef _gkg_processing_plugin_functors_IntensityNormalization_IntensityNormalizationCommand_h_
#define _gkg_processing_plugin_functors_IntensityNormalization_IntensityNormalizationCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class IntensityNormalizationCommand :
                              public Command,
                              public Creator2Arg< IntensityNormalizationCommand,
                                         	  Command,
                                         	  int32_t,
                                         	  char** >,
                              public Creator1Arg< IntensityNormalizationCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    IntensityNormalizationCommand( int32_t argc,
                  		   char* argv[], 
                  		   bool loadPlugin = false,
                  		   bool removeFirst = true );
    IntensityNormalizationCommand( const std::string& fileNameVolumeIn,
                                   const std::string& fileNameVolumeOut,
                                   const std::string& fileNameVolumeWeight,
                                   bool verbose );
    IntensityNormalizationCommand( const Dictionary& parameters );
    virtual ~IntensityNormalizationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< IntensityNormalizationCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< IntensityNormalizationCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameVolumeIn,
                  const std::string& fileNameVolumeOut,
                  const std::string& fileNameVolumeWeight,
                  bool verbose );

};


}


#endif
