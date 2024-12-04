#include "ASpinGlassShader.h"
#include <gkg-core-exception/Exception.h>


std::string gkg::shaderCodeSpinGlass(
                              const std::string& spinGlassExtremityCountString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " uniform int isShaderedSpinGlassObject; ";
    shaderCode += " uniform vec3 spinGlassExtremities[ ";
    shaderCode += spinGlassExtremityCountString;
    shaderCode += " ]; ";
    shaderCode += "uniform vec3 spinGlassColorValues [ ";
    shaderCode += spinGlassExtremityCountString;
    shaderCode += " ];                                                         \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedSpinGlassObject == 1 )                                  \
        {                                                                      \
                                                                               \
          int index = int( gl_Vertex.x );                                      \
          gl_FrontColor = vec4( spinGlassColorValues[ index ].x,               \
                                spinGlassColorValues[ index ].y,               \
                                spinGlassColorValues[ index ].z,               \
                                1.0 );                                         \
          gl_Position.x = spinGlassExtremities[ index ].x;                     \
          gl_Position.y = spinGlassExtremities[ index ].y;                     \
          gl_Position.z = spinGlassExtremities[ index ].z;                     \
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
  GKG_CATCH( "std::string gkg::shaderCodeSpinGlass( "
             "const std::string& spinGlassExtremityCountString )" );

}
