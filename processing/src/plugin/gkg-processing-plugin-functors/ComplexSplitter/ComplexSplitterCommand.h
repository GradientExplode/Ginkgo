#ifndef _gkg_processing_plugin_functors_ComplexSplitter_ComplexSplitterCommand_h_
#define _gkg_processing_plugin_functors_ComplexSplitter_ComplexSplitterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class ComplexSplitterCommand : public Command,
                               public Creator2Arg< ComplexSplitterCommand,
                                		   Command,
                                		   int32_t,
                                		   char** >,
                               public Creator1Arg< ComplexSplitterCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    ComplexSplitterCommand( int32_t argc,
                            char* argv[], 
                            bool loadPlugin = false,
                            bool removeFirst = true );
    ComplexSplitterCommand( const std::string& fileNameIn,
                            std::string& fileNameOut, 
                            bool verbose );
    ComplexSplitterCommand( const Dictionary& parameters );
    virtual ~ComplexSplitterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< ComplexSplitterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< ComplexSplitterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  std::string& fileNameOut, 
                  bool verbose );

};


}


#endif
