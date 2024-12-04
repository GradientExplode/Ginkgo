#include "ASpinGlassConnectionShader.h"
#include <gkg-core-exception/Exception.h>


std::string gkg::shaderCodeSpinGlassConnection(
                    const std::string& spinGlassConnectionExtremityCountString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " uniform int isShaderedSpinGlassConnectionObject; ";
    shaderCode += " uniform vec3 spinGlassConnectionExtremities[ ";
    shaderCode += spinGlassConnectionExtremityCountString;
    shaderCode += " ]; ";
    shaderCode += "uniform vec3 spinGlassConnectionColorValues [ ";
    shaderCode += spinGlassConnectionExtremityCountString;
    shaderCode += " ];                                                         \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedSpinGlassConnectionObject == 1 )                        \
        {                                                                      \
                                                                               \
          int index = int( gl_Vertex.x );                                      \
          gl_FrontColor = vec4( spinGlassConnectionColorValues[ index ].x,     \
                                spinGlassConnectionColorValues[ index ].y,     \
                                spinGlassConnectionColorValues[ index ].z,     \
                                1.0 );                                         \
          gl_Position.x = spinGlassConnectionExtremities[ index ].x;           \
          gl_Position.y = spinGlassConnectionExtremities[ index ].y;           \
          gl_Position.z = spinGlassConnectionExtremities[ index ].z;           \
          gl_Position.w = gl_Vertex.w;                                         \
          gl_Position = gl_ModelViewProjectionMatrix * ( gl_Position ) ;       \
                                                                               \
        }                                                                      \
        else                                                                   \
        {                                                                      \
                                                                               \
          gl_FrontColor = gl_Color;                                            \
          gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;              \
                                                                               \
        }                                                                      \
                                                                               \
      } ";

    return shaderCode;

  }
  GKG_CATCH( "std::string gkg::shaderCodeSpinGlassConnection( "
             "const std::string& spinGlassConnectionExtremityCountString )" );

}
