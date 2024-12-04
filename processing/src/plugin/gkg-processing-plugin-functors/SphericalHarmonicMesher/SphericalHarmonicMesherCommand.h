#ifndef _gkg_processing_plugin_functors_SphericalHarmonicMesher_SphericalHarmonicMesherCommand_h_
#define _gkg_processing_plugin_functors_SphericalHarmonicMesher_SphericalHarmonicMesherCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SphericalHarmonicMesherCommand :
                             public Command,
                             public Creator2Arg< SphericalHarmonicMesherCommand,
                                        	 Command,
                                        	 int32_t,
                                        	 char** >,
                             public Creator1Arg< SphericalHarmonicMesherCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    SphericalHarmonicMesherCommand( int32_t argc,
                  		    char* argv[], 
                  		    bool loadPlugin = false,
                  		    bool removeFirst = true );
    SphericalHarmonicMesherCommand( int32_t j,
                                    int32_t l,
                                    int32_t m,
                                    const std::string& fileNameOut,
                                    int32_t meshPointCount,
                                    const std::string& meshMapFormat,
                                    const std::string& textureMapFormat,
                                    bool ascii,
                                    bool verbose,
                                    bool minMax );
    SphericalHarmonicMesherCommand( const Dictionary& parameters );
    virtual ~SphericalHarmonicMesherCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SphericalHarmonicMesherCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SphericalHarmonicMesherCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( int32_t j,
                  int32_t l,
                  int32_t m,
                  const std::string& fileNameOut,
                  int32_t meshPointCount,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  bool ascii,
                  bool verbose,
                  bool minMax );

};


}


#endif
