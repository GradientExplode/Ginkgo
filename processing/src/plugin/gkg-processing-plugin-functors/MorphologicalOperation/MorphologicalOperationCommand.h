#ifndef _gkg_processing_plugin_functors_MorphologicalOperation_MorphologicalOperationCommand_h_
#define _gkg_processing_plugin_functors_MorphologicalOperation_MorphologicalOperationCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MorphologicalOperationCommand :
                              public Command,
                              public Creator2Arg< MorphologicalOperationCommand,
                                         	  Command,
                                         	  int32_t,
                                         	  char** >,
                              public Creator1Arg< MorphologicalOperationCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    MorphologicalOperationCommand( int32_t argc,
                 		   char* argv[], 
                 		   bool loadPlugin = false,
                 		   bool removeFirst = true );
    MorphologicalOperationCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& operation,
                                   float radius,
                                   int32_t neighborhood,
                                   const std::string& outType,
                                   const std::string& mode,
                                   double threshold1,
                                   double threshold2,
                                   double foreground,
                                   double background,
                                   bool ascii,
                                   const std::string& format,
                                   bool verbose );
    MorphologicalOperationCommand( const Dictionary& parameters );
    virtual ~MorphologicalOperationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MorphologicalOperationCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MorphologicalOperationCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& operation,
                  float radius,
                  int32_t neighborhood,
                  const std::string& outType,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double foreground,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
