#ifndef _gkg_dmri_plugin_functors_DwiFiberMeasure_DwiFiberMeasureCommand_h_
#define _gkg_dmri_plugin_functors_DwiFiberMeasure_DwiFiberMeasureCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiFiberMeasureCommand : public Command,
                               public Creator2Arg< DwiFiberMeasureCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< DwiFiberMeasureCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiFiberMeasureCommand( int32_t argc,
                            char* argv[],
                            bool loadPlugin = false,
                            bool removeFirst = true );
    DwiFiberMeasureCommand( const std::string& fileNameBundleMap,
                            const std::string& fileNameMeasure,
                            const std::vector< std::string >& measureNames,
                            const std::vector< std::string >& stringParameters,
                            const std::vector< double >& scalarParameters,
                            bool verbose );
    DwiFiberMeasureCommand( const Dictionary& parameters );
    virtual ~DwiFiberMeasureCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiFiberMeasureCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiFiberMeasureCommand, Command,
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
