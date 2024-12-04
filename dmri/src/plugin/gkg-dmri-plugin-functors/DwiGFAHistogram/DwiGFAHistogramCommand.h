#ifndef _gkg_dmri_plugin_functors_DwiGFAHistogram_DwiGFAHistogramCommand_h_
#define _gkg_dmri_plugin_functors_DwiGFAHistogram_DwiGFAHistogramCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiGFAHistogramCommand : public Command,
                               public Creator2Arg< DwiGFAHistogramCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< DwiGFAHistogramCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiGFAHistogramCommand( int32_t argc,
                            char* argv[],
                            bool loadPlugin = false,
                            bool removeFirst = true );
    DwiGFAHistogramCommand( const std::vector< std::string >& fileNameGFAs,
                            const std::string& fileNameMask,
                            const std::string& fileNameOutput,
                            bool cumulative,
                            bool verbose );
    DwiGFAHistogramCommand( const Dictionary& parameters );
    virtual ~DwiGFAHistogramCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiGFAHistogramCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiGFAHistogramCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameGFAs,
                  const std::string& fileNameMask,
                  const std::string& fileNameOutput,
                  bool cumulative,
                  bool verbose );

};


}


#endif
