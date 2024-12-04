#ifndef _gkg_dmri_plugin_functors_DwiBundleOperator_DwiBundleOperatorCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleOperator_DwiBundleOperatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiBundleOperatorCommand : public Command,
                                 public Creator2Arg< DwiBundleOperatorCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< DwiBundleOperatorCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DwiBundleOperatorCommand( int32_t argc,
                              char* argv[],
                              bool loadPlugin = false,
                              bool removeFirst = true );
    DwiBundleOperatorCommand(
                     const std::vector< std::string >& fileNameInputBundleMaps,
                     const std::vector< std::string >& fileNameOutputBundleMaps,
                     const std::string& operatorName,
                     const std::vector< std::string >& stringParameters,
                     const std::vector< double >& scalarParameters,
                     const std::string& outputFormat,
                     bool ascii,
                     bool verbose );
    DwiBundleOperatorCommand( const Dictionary& parameters );
    virtual ~DwiBundleOperatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleOperatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleOperatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameInputBundleMaps,
                  const std::vector< std::string >& fileNameOutputBundleMaps,
                  const std::string& operatorName,
                  const std::vector< std::string >& stringParameters,
                  const std::vector< double >& scalarParameters,
                  const std::string& outputFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
