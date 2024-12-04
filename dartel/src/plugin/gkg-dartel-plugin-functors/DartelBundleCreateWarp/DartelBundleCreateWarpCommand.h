#ifndef _gkg_dartel_plugin_functors_DartelBundleCreateWarp_DartelBundleCreateWarpCommand_h_
#define _gkg_dartel_plugin_functors_DartelBundleCreateWarp_DartelBundleCreateWarpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DartelBundleCreateWarpCommand : public Command,
                                      public Creator2Arg<
                                                  DartelBundleCreateWarpCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                                      public Creator1Arg<
                                                  DartelBundleCreateWarpCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DartelBundleCreateWarpCommand( int32_t argc,
                                   char* argv[], 
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    DartelBundleCreateWarpCommand( const std::string& fileNameIn,
                                   const std::string& fileNameInTemplate,
                                   const std::string& fileNameFlowField,
                                   const std::string& fileNameOut,
                                   const std::string& fileNameTemplate,
                                   int32_t k,
                                   bool inverse,
                                   bool verbose );
    DartelBundleCreateWarpCommand( const Dictionary& parameters );
    virtual ~DartelBundleCreateWarpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DartelBundleCreateWarpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DartelBundleCreateWarpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameInTemplate,
                  const std::string& fileNameFlowField,
                  const std::string& fileNameOut,
                  const std::string& fileNameTemplate,
                  int32_t k,
                  bool inverse,
    		  bool verbose );

};


}


#endif
