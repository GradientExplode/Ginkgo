#ifndef _gkg_dmri_plugin_functors_DwiBundleMapHierarchy_DwiBundleMapHierarchyCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapHierarchy_DwiBundleMapHierarchyCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapHierarchyCommand :
                               public Command,
                               public Creator2Arg< DwiBundleMapHierarchyCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< DwiBundleMapHierarchyCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiBundleMapHierarchyCommand( int32_t argc,
                                  char* argv[], 
                                  bool loadPlugin = false,
                                  bool removeFirst = true );
    DwiBundleMapHierarchyCommand( const std::string& fileNameBundleMap,
                                  const std::string& fileNameOut,
                                  bool verbose );
    DwiBundleMapHierarchyCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapHierarchyCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapHierarchyCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapHierarchyCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
