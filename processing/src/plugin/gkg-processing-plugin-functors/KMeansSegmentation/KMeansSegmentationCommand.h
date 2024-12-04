#ifndef _gkg_processing_plugin_functors_KMeansSegmentation_KMeansSegmentation_h_
#define _gkg_processing_plugin_functors_KMeansSegmentation_KMeansSegmentation_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class KMeansSegmentationCommand :
                           public Command,
                           public Creator2Arg< KMeansSegmentationCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< KMeansSegmentationCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    KMeansSegmentationCommand(
                               int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    KMeansSegmentationCommand( const std::string& fileNameVolumeIn,
                               const std::string& fileNameVolumeMask,
                               const std::string& fileNameVolumeOut, 
                               int32_t clusterCount,
                               int32_t maximumIterationCount,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    KMeansSegmentationCommand( const Dictionary& parameters );
    virtual ~KMeansSegmentationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< KMeansSegmentationCommand,
                              Command, int32_t, char** >;
    friend class Creator1Arg< KMeansSegmentationCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameVolumeIn,
                  const std::string& fileNameVolumeMask,
                  const std::string& fileNameVolumeOut, 
                  int32_t clusterCount,
                  int32_t maximumIterationCount,
                  bool ascii,
                  const std::string& format,
                  bool verbose );


};


}


#endif
