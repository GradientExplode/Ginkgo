#ifndef _gkg_processing_plugin_functors_MannWhitneyWilcoxonUTest_MannWhitneyWilcoxonUTestCommand_h_
#define _gkg_processing_plugin_functors_MannWhitneyWilcoxonUTest_MannWhitneyWilcoxonUTestCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MannWhitneyWilcoxonUTestCommand :
                            public Command,
                            public Creator2Arg< MannWhitneyWilcoxonUTestCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< MannWhitneyWilcoxonUTestCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    MannWhitneyWilcoxonUTestCommand( int32_t argc,
                  		     char* argv[], 
                  		     bool loadPlugin = false,
                  		     bool removeFirst = true );
    MannWhitneyWilcoxonUTestCommand( const std::string& fileNameSample1,
                                     const std::string& fileNameSample2,
                                     const std::string& fileNameOut,
                                     bool verbose );
    MannWhitneyWilcoxonUTestCommand( const Dictionary& parameters );
    virtual ~MannWhitneyWilcoxonUTestCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MannWhitneyWilcoxonUTestCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MannWhitneyWilcoxonUTestCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameSample1,
                  const std::string& fileNameSample2,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
