#ifndef _gkg_processing_plugin_functors_Mask2HemisphereMask_Mask2HemisphereMaskCommand_h_
#define _gkg_processing_plugin_functors_Mask2HemisphereMask_Mask2HemisphereMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Mask2HemisphereMaskCommand :
                           public Command,
                     	   public Creator2Arg< Mask2HemisphereMaskCommand,
                     	   		       Command,
                     	   		       int32_t,
                     	   		       char** >,
                           public Creator1Arg< Mask2HemisphereMaskCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    Mask2HemisphereMaskCommand( int32_t argc,
                  	char* argv[], 
                  	bool loadPlugin = false,
                  	bool removeFirst = true );
    Mask2HemisphereMaskCommand( const std::string& fileNameIn,
                        const std::string& fileNameOut,
                        int32_t middle,
                        double background,
                        bool ascii,
                        const std::string& format,
                        bool verbose );
    Mask2HemisphereMaskCommand( const Dictionary& parameters );
    virtual ~Mask2HemisphereMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Mask2HemisphereMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Mask2HemisphereMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t middle,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
