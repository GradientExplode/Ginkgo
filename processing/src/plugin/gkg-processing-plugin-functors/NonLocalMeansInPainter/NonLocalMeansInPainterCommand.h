#ifndef _gkg_processing_plugin_functors_NonLocalMeansInPainter_NonLocalMeansInPainterCommand_h_
#define _gkg_processing_plugin_functors_NonLocalMeansInPainter_NonLocalMeansInPainterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class NonLocalMeansInPainterCommand :
                              public Command,
                              public Creator2Arg< NonLocalMeansInPainterCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< NonLocalMeansInPainterCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    NonLocalMeansInPainterCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    NonLocalMeansInPainterCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& fileNameMask,
                                   const std::string& fileNameInPaintingMask,
                                   const std::string& noiseModel,
                                   int32_t halfSearchBlockSize,
                                   int32_t halfNeighborhoodSize,
                                   float degreeOfFiltering,
                                   float noiseStdDev,
                                   float coilCount,
                                   bool ascii,
                                   const std::string& format,
                                   bool verbose );
    NonLocalMeansInPainterCommand( const Dictionary& parameters );
    virtual ~NonLocalMeansInPainterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< NonLocalMeansInPainterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< NonLocalMeansInPainterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameMask,
                  const std::string& fileNameInPaintingMask,
                  const std::string& noiseModel,
                  int32_t halfSearchBlockSize,
                  int32_t halfNeighborhoodSize,
                  float degreeOfFiltering,
                  float noiseStdDev,
                  float coilCount,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
