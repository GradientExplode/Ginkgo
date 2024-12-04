#ifndef _gkg_processing_plugin_functors_HoleFilling2d_HoleFilling2dCommand_h_
#define _gkg_processing_plugin_functors_HoleFilling2d_HoleFilling2dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class HoleFilling2dCommand : public Command,
                             public Creator2Arg< HoleFilling2dCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< HoleFilling2dCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    HoleFilling2dCommand( int32_t argc,
                  	  char* argv[], 
                  	  bool loadPlugin = false,
                  	  bool removeFirst = true );
    HoleFilling2dCommand( const std::string& fileNameIn,
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
    HoleFilling2dCommand( const Dictionary& parameters );
    virtual ~HoleFilling2dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< HoleFilling2dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< HoleFilling2dCommand, Command,
                              const Dictionary& >;

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
