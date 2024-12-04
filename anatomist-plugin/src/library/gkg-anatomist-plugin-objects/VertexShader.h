#ifndef _gkg_anatomist_plugin_objects_VertexShader_h_
#define _gkg_anatomist_plugin_objects_VertexShader_h_


#include <string>


namespace gkg
{


std::string shaderCode( const std::string& outputOrientationCountString,
                        const std::string& minimumResolutionString );
std::string shaderCodeWithApproximatedNormals( 
                        const std::string& outputOrientationCountString,
                        const std::string& minimumResolutionString );
std::string shaderCodeWithoutNormals( 
                        const std::string& outputOrientationCountString,
                        const std::string& minimumResolutionString );


}


#endif
