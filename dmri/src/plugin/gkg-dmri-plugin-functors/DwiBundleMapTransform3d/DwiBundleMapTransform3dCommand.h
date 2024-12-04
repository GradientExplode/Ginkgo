#ifndef _gkg_dmri_plugin_functors_DwiBundleMapTransform3d_DwiBundleMapTransform3dCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapTransform3d_DwiBundleMapTransform3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapTransform3dCommand :
                             public Command,
                             public Creator2Arg< DwiBundleMapTransform3dCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DwiBundleMapTransform3dCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DwiBundleMapTransform3dCommand( int32_t argc,
                                    char* argv[],
                                    bool loadPlugin = false,
                                    bool removeFirst = true );
    DwiBundleMapTransform3dCommand( const std::string& fileNameBundleMap,
                                    const std::string& fileNameOut,
                                    const std::string& fileNameTransform3d,
                                    bool verbose );
    DwiBundleMapTransform3dCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapTransform3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapTransform3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapTransform3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameOut,
                  const std::string& fileNameTransform3d,
                  bool verbose );

};


}


#endif
