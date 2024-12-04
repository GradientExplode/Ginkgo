#ifndef _gkg_dmri_plugin_functors_DwiBundleMeasure_DwiBundleMeasureCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMeasure_DwiBundleMeasureCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiBundleMeasureCommand : public Command,
                                public Creator2Arg< DwiBundleMeasureCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiBundleMeasureCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiBundleMeasureCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin = false,
                             bool removeFirst = true );
    DwiBundleMeasureCommand( const std::string& fileNameBundleMap,
                             const std::string& fileNameMeasure,
                             const std::vector< std::string >& measureNames,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );
    DwiBundleMeasureCommand( const Dictionary& parameters );
    virtual ~DwiBundleMeasureCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMeasureCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMeasureCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameMeasure,
                  const std::vector< std::string >& measureNames,
                  const std::vector< std::string >& stringParameters,
                  const std::vector< double >& scalarParameters,
                  bool verbose );

};


}


#endif
