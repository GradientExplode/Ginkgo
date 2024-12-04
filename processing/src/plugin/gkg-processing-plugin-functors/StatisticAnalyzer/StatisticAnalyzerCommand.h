#ifndef _gkg_processing_plugin_functors_StatisticAnalyzer_StatisticAnalyzerCommand_h_
#define _gkg_processing_plugin_functors_StatisticAnalyzer_StatisticAnalyzerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class StatisticAnalyzerCommand : public Command,
                                 public Creator2Arg< StatisticAnalyzerCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< StatisticAnalyzerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    StatisticAnalyzerCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    StatisticAnalyzerCommand( const std::string& fileNameIn );
    StatisticAnalyzerCommand( const Dictionary& parameters );
    virtual ~StatisticAnalyzerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< StatisticAnalyzerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< StatisticAnalyzerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn );

};


}


#endif
