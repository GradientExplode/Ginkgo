#ifndef _gkg_dmri_plugin_functors_DwiBundleMapConvexHull_DwiBundleMapConvexHullCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapConvexHull_DwiBundleMapConvexHullCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapConvexHullCommand :
                             public Command,
                             public Creator2Arg< DwiBundleMapConvexHullCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DwiBundleMapConvexHullCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DwiBundleMapConvexHullCommand( int32_t argc,
                  char* argv[], 
                  bool loadPlugin = false,
                  bool removeFirst = true );
    DwiBundleMapConvexHullCommand( const std::string& fileNameBundleMap,
                                   const std::string& fileNameOut,
                                   float step,
                                   const std::string& outputFormat,
                                   bool singleFile,
                                   const bool& useThickFascicles,
                                   const float& fascicleRadius,
                                   const int32_t& sphereOrientationCount,
                                   bool ascii,
                                   bool verbose );
    DwiBundleMapConvexHullCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapConvexHullCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapConvexHullCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapConvexHullCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                   const std::string& fileNameOut,
                   float step,
                   const std::string& outputFormat,
                   bool singleFile,
                   const bool& useThickFascicles,
                   const float& fascicleRadius,
                   const int32_t& sphereOrientationCount,
                   bool ascii,
                   bool verbose );

};


}


#endif
