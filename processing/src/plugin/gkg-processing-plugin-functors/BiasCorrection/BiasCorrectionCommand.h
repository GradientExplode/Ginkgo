#ifndef _gkg_processing_plugin_functors_BiasCorrection_BiasCorrectionCommand_h_
#define _gkg_processing_plugin_functors_BiasCorrection_BiasCorrectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class BiasCorrectionCommand : public Command,
                              public Creator2Arg< BiasCorrectionCommand,
                                		  Command,
                                		  int32_t,
                                		  char** >,
                              public Creator1Arg< BiasCorrectionCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    BiasCorrectionCommand( int32_t argc,
                              char* argv[], 
                              bool loadPlugin = false,
                              bool removeFirst = true );
    BiasCorrectionCommand( const std::string& fileNameIn,
                           const std::string& fileNameMask,
                           const std::string& fileNameWeight,
                           const std::string& fileNameOut,
                           const std::string& fileNameBias,
                           const std::string& type,
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters,
                           bool ascii,
                           const std::string& format,
                           bool verbose );
    BiasCorrectionCommand( const Dictionary& parameters );
    virtual ~BiasCorrectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BiasCorrectionCommand, Command, int32_t, char** >;
    friend class Creator1Arg< BiasCorrectionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameMask,
                  const std::string& fileNameWeight,
                  const std::string& fileNameOut,
                  const std::string& fileNameBias,
                  const std::string& type,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
