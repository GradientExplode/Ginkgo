#ifndef _gkg_processing_plugin_functors_VolumeToMeshIntersection_VolumeToMeshIntersectionCommand_h_
#define _gkg_processing_plugin_functors_VolumeToMeshIntersection_VolumeToMeshIntersectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VolumeToMeshIntersectionCommand :
                            public Command,
                            public Creator2Arg< VolumeToMeshIntersectionCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< VolumeToMeshIntersectionCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    VolumeToMeshIntersectionCommand( int32_t argc,
                  		     char* argv[], 
                  		     bool loadPlugin = false,
                  		     bool removeFirst = true );
    VolumeToMeshIntersectionCommand( const std::string& fileNameMesh,
                                     const std::string& fileNameVolume,
    				     const std::string& fileNameTransform3d,
    				     const std::string& fileNameMask,
    				     const std::string& fileNameOut,
    				     double outerDepth,
    				     double innerDepth,
    				     double resamplingStep,
    				     int32_t resamplingOrder,
    				     const std::string& operation,
    				     bool ascii,
    				     const std::string& format,
    				     bool verbose );
    VolumeToMeshIntersectionCommand( const Dictionary& parameters );
    virtual ~VolumeToMeshIntersectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeToMeshIntersectionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VolumeToMeshIntersectionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMesh,
                  const std::string& fileNameVolume,
    		  const std::string& fileNameTransform3d,
    		  const std::string& fileNameMask,
    		  const std::string& fileNameOut,
    		  double outerDepth,
    		  double innerDepth,
    		  double resamplingStep,
    		  int32_t resamplingOrder,
    		  const std::string& operation,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
