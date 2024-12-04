#ifndef _gkg_deep_nuclei_plugin_functors_MidSagittalSurface_MidSagittalSurfaceCommand_h_
#define _gkg_deep_nuclei_plugin_functors_MidSagittalSurface_MidSagittalSurfaceCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MidSagittalSurfaceCommand : public Command,
                                  public Creator2Arg< MidSagittalSurfaceCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< MidSagittalSurfaceCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    MidSagittalSurfaceCommand( int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    MidSagittalSurfaceCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               const std::string& fileNameMask,
                               int32_t slabWidth,
                               double sigma,
                               double minCorrelation,
                               double lambda,
			       bool output3d,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    MidSagittalSurfaceCommand( const Dictionary& parameters );
    virtual ~MidSagittalSurfaceCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MidSagittalSurfaceCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MidSagittalSurfaceCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameMask,
                  int32_t slabWidth,
                  double sigma,
                  double minCorrelation,
                  double lambda,
		  bool output3d,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
