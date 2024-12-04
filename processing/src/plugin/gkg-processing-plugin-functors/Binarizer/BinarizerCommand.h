#ifndef _gkg_processing_plugin_functors_Binarizer_BinarizerCommand_h_
#define _gkg_processing_plugin_functors_Binarizer_BinarizerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BinarizerCommand : public Command,
                         public Creator2Arg< BinarizerCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< BinarizerCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    BinarizerCommand( int32_t argc,
                      char* argv[], 
                      bool loadPlugin = false,
                      bool removeFirst = true );
    BinarizerCommand( const std::string& fileNameIn,
                      const std::string& fileNameOut,
                      const std::string& outType,
                      const std::string& mode,
                      double threshold1,
                      double threshold2,
                      double foreground,
                      double background,
                      bool ascii,
                      const std::string& format,
                      bool verbose );
    BinarizerCommand( const Dictionary& parameters );
    virtual ~BinarizerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BinarizerCommand, Command, int32_t, char** >;
    friend class Creator1Arg< BinarizerCommand, Command, const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
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
