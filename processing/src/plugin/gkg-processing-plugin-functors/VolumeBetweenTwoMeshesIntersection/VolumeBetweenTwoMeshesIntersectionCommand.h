#ifndef _gkg_processing_plugin_functors_VolumeBetweenTwoMeshesIntersection_VolumeBetweenTwoMeshesIntersectionCommand_h_
#define _gkg_processing_plugin_functors_VolumeBetweenTwoMeshesIntersection_VolumeBetweenTwoMeshesIntersectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VolumeBetweenTwoMeshesIntersectionCommand :
                  public Command,
                  public Creator2Arg< VolumeBetweenTwoMeshesIntersectionCommand,
                                      Command,
                                      int32_t,
                                      char** >,
                  public Creator1Arg< VolumeBetweenTwoMeshesIntersectionCommand,
                                      Command,
                                      const Dictionary& >
{

  public:

    VolumeBetweenTwoMeshesIntersectionCommand( int32_t argc,
                  			       char* argv[], 
                  			       bool loadPlugin = false,
                  			       bool removeFirst = true );
    VolumeBetweenTwoMeshesIntersectionCommand( 
    					 const std::string& fileNameVolume,
    					 const std::string& fileNameOuterMesh,
    					 const std::string& fileNameInnerMesh,
    					 const std::string& fileNameTransform3d,
    					 const std::string& fileNameMask,
    					 const std::string& fileNameOut,
    					 float resamplingStep,
    					 int32_t resamplingOrder,
    					 bool ascii,
    					 const std::string& format,
    					 bool verbose );
    VolumeBetweenTwoMeshesIntersectionCommand( const Dictionary& parameters );
    virtual ~VolumeBetweenTwoMeshesIntersectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeBetweenTwoMeshesIntersectionCommand,
                              Command, 
                              int32_t,
                              char** >;
    friend class Creator1Arg< VolumeBetweenTwoMeshesIntersectionCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameVolume,
    		  const std::string& fileNameOuterMesh,
    		  const std::string& fileNameInnerMesh,
    		  const std::string& fileNameTransform3d,
    		  const std::string& fileNameMask,
    		  const std::string& fileNameOut,
    		  float resamplingStep,
    		  int32_t resamplingOrder,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
