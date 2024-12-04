#ifndef _gkg_qmri_plugin_functors_RelaxometryMapper_RelaxometryMapperCommand_h_
#define _gkg_qmri_plugin_functors_RelaxometryMapper_RelaxometryMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class RelaxometryMapperCommand : public Command,
                                 public Creator2Arg< RelaxometryMapperCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< RelaxometryMapperCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    RelaxometryMapperCommand( int32_t argc,
                              char* argv[], 
                              bool loadPlugin = false,
                              bool removeFirst = true );
    RelaxometryMapperCommand( const std::string& fileNameIn, 
                              const std::string& fileNameTimes,
                              const std::string& fileNameMask,
                              const std::string& fileNameOut,
                              const std::string& fileNameFittedData,
                              const std::string& relaxometryType,
                              const std::string& optimizerType,
                              std::vector< double >& optimizerParameters,
                              int32_t concatenations,
                              double repetitionTimePerSlice,
                              std::vector< double >& outlierValues,
                              bool ascii,
                              const std::string& format,
                              bool verbose );
    RelaxometryMapperCommand( const Dictionary& parameters );
    virtual ~RelaxometryMapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RelaxometryMapperCommand, Command,
                              int32_t, char** >;
    friend class Creator1Arg< RelaxometryMapperCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn, 
                  const std::string& fileNameTimes,
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  const std::string& fileNameFittedData,
                  const std::string& relaxometryType,
                  const std::string& optimizerType,
                  std::vector< double >& optimizerParameters,
                  int32_t concatenations,
                  double repetitionTimePerSlice,
                  std::vector< double >& outlierValues,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
