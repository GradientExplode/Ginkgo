#ifndef _gkg_processing_plugin_functors_Voronoi_VoronoiCommand_h_
#define _gkg_processing_plugin_functors_Voronoi_VoronoiCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VoronoiCommand : public Command,
                       public Creator2Arg< VoronoiCommand,
                                           Command,
                    		 	   int32_t,
                    			   char** >,
                       public Creator1Arg< VoronoiCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    VoronoiCommand( int32_t argc,
                    char* argv[], 
                    bool loadPlugin = false,
                    bool removeFirst = true );
    VoronoiCommand( const std::string& fileNameIn,
                    const std::string& fileNameOut,
                    const std::string& outType,
                    const std::string& insideMode,
                    const double& insideThreshold1,
                    const double& insideThreshold2,
                    const std::string& outsideMode,
                    const double& outsideThreshold1,
                    const double& outsideThreshold2,
                    const double& outsideValue,
                    const bool& ascii,
                    const std::string& format,
                    const bool& verbose );
    VoronoiCommand( const Dictionary& parameters );
    virtual ~VoronoiCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VoronoiCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VoronoiCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outType,
                  const std::string& insideMode,
                  const double& insideThreshold1,
                  const double& insideThreshold2,
                  const std::string& outsideMode,
                  const double& outsideThreshold1,
                  const double& outsideThreshold2,
                  const double& outsideValue,
                  const bool& ascii,
                  const std::string& format,
                  const bool& verbose );

};


}


#endif
