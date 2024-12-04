#ifndef _gkg_processing_plugin_functors_VolumeRootMeanSquareDifference_VolumeRootMeanSquareDifferenceCommand_h_
#define _gkg_processing_plugin_functors_VolumeRootMeanSquareDifference_VolumeRootMeanSquareDifferenceCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class VolumeRootMeanSquareDifferenceCommand :
                      public Command,
                      public Creator2Arg< VolumeRootMeanSquareDifferenceCommand,
                        	          Command,
                                          int32_t,
                                          char** >,
                      public Creator1Arg< VolumeRootMeanSquareDifferenceCommand,
                                          Command,
                                          const Dictionary& >
{

  public:

    VolumeRootMeanSquareDifferenceCommand( int32_t argc,
                                           char* argv[],
                                           bool loadPlugin = false,
                                           bool removeFirst = true );
    VolumeRootMeanSquareDifferenceCommand(
                                  const std::string& fileNameVolumeReference,
                                  const std::string& fileNameVolumeTest,
                                  const std::string& fileNameAbsoluteDifference,
                                  const std::string& fileNameSignedDifference,
                                  const std::string& fileNameMask,
                                  const std::string& mode,
                                  double threshold1,
                                  double threshold2,
                                  bool ascii,
                                  const std::string& format,
                                  bool verbose );
    VolumeRootMeanSquareDifferenceCommand( const Dictionary& parameters );
    virtual ~VolumeRootMeanSquareDifferenceCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeRootMeanSquareDifferenceCommand,
                              Command,
                              int32_t,
                              char** >;
    friend class Creator1Arg< VolumeRootMeanSquareDifferenceCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameVolumeReference,
                  const std::string& fileNameVolumeTest,
                  const std::string& fileNameAbsoluteDifference,
                  const std::string& fileNameSignedDifference,
                  const std::string& fileNameMask,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
