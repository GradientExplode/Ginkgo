#ifndef _gkg_dmri_plugin_functors_DwiFiberFilter_DwiFiberFilterCommand_h_
#define _gkg_dmri_plugin_functors_DwiFiberFilter_DwiFiberFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <list>
#include <vector>


namespace gkg
{


class DwiFiberFilterCommand : public Command,
                              public Creator2Arg< DwiFiberFilterCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DwiFiberFilterCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DwiFiberFilterCommand( int32_t argc,
                           char* argv[],
                           bool loadPlugin = false,
                           bool removeFirst = true );
    DwiFiberFilterCommand( 
                          const std::list< std::string >& fileNameBundleMapIns,
                          const std::list< std::string >& fileNameBundleMapOuts,
                          const std::vector< std::string >& selectorNames,
                          const std::vector< std::string >& stringParameters,
                          const std::vector< double >& scalarParameters,
                          const std::string& bundleMapFormat,
                          bool ascii,
                          bool verbose );
    DwiFiberFilterCommand( const Dictionary& parameters );
    virtual ~DwiFiberFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiFiberFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiFiberFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::list< std::string >& fileNameBundleMapIns,
                  const std::list< std::string >& fileNameBundleMapOuts,
                  const std::vector< std::string >& selectorNames,
                  const std::vector< std::string >& stringParameters,
                  const std::vector< double >& scalarParameters,
                  const std::string& bundleMapFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
