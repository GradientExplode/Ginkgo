#ifndef _gkg_processing_plugin_functors_NonLocalMeansFilter_NonLocalMeansFilterCommand_h_
#define _gkg_processing_plugin_functors_NonLocalMeansFilter_NonLocalMeansFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class NonLocalMeansFilterCommand :
                                 public Command,
                                 public Creator2Arg< NonLocalMeansFilterCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< NonLocalMeansFilterCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    NonLocalMeansFilterCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    NonLocalMeansFilterCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                const std::string& noiseModel,
                                int32_t halfSearchBlocSize,
                                int32_t halfNeighborhoodSize,
                                float degreeOfFiltering,
                                float sigma,
                                float n,
                                bool ascii,
                                const std::string& format,
                                bool verbose );
    NonLocalMeansFilterCommand( const Dictionary& parameters );
    virtual ~NonLocalMeansFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< NonLocalMeansFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< NonLocalMeansFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& noiseModel,
                  int32_t halfSearchBlocize,
                  int32_t halfNeighborhoodSize,
                  float degreeOfFiltering,
                  float sigma,
                  float n,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
