#ifndef _gkg_dmri_plugin_functors_DwiSingleSubjectTrackDensityImaging_DwiSingleSubjectTrackDensityImagingCommand_h_
#define _gkg_dmri_plugin_functors_DwiSingleSubjectTrackDensityImaging_DwiSingleSubjectTrackDensityImagingCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiSingleSubjectTrackDensityImagingCommand :
                 public Command,
                 public Creator2Arg< DwiSingleSubjectTrackDensityImagingCommand,
                                     Command,
                                     int32_t,
                                     char** >,
                 public Creator1Arg< DwiSingleSubjectTrackDensityImagingCommand,
                                     Command,
                                     const Dictionary& >
{

  public:

    DwiSingleSubjectTrackDensityImagingCommand( int32_t argc,
                                                char* argv[],
                                                bool loadPlugin = false,
                                                bool removeFirst = true );
    DwiSingleSubjectTrackDensityImagingCommand( 
                           const std::vector< std::string >& fileNameBundleMaps,
                           const std::vector< double >& outputResolutionVector,
                           const std::string& fileNameInputVolume,
                           const std::string& fileNameTransform3d,
                           const std::string& fileNameOutputVolume,
                           float step,
                           const std::string& outputFormat,
                           bool ascii,
                           bool verbose );
    DwiSingleSubjectTrackDensityImagingCommand( const Dictionary& parameters );
    virtual ~DwiSingleSubjectTrackDensityImagingCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSingleSubjectTrackDensityImagingCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSingleSubjectTrackDensityImagingCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameBundleMaps,
                  const std::vector< double >& outputResolutionVector,
                  const std::string& fileNameInputVolume,
                  const std::string& fileNameTransform3d,
                  const std::string& fileNameOutputVolume,
                  float step,
                  const std::string& outputFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
