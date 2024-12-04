#ifndef _gkg_processing_plugin_functors_T2ToT1ContrastTransformer_T2ToT1ContrastTransformerCommand_h_
#define _gkg_processing_plugin_functors_T2ToT1ContrastTransformer_T2ToT1ContrastTransformerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class T2ToT1ContrastTransformerCommand :
                         public Command,
                         public Creator2Arg< T2ToT1ContrastTransformerCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< T2ToT1ContrastTransformerCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    T2ToT1ContrastTransformerCommand( int32_t argc,
                      char* argv[], 
                      bool loadPlugin = false,
                      bool removeFirst = true );
    T2ToT1ContrastTransformerCommand( const std::string& fileNameIn,
                      const std::string& fileNameOut,
                      double lowerThreshold,
                      double upperThreshold,
                      bool ascii,
                      const std::string& format,
                      bool verbose );
    T2ToT1ContrastTransformerCommand( const Dictionary& parameters );
    virtual ~T2ToT1ContrastTransformerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< T2ToT1ContrastTransformerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< T2ToT1ContrastTransformerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  double lowerThreshold,
                  double upperThreshold,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
