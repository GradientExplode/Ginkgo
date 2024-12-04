#include <gkg-anatomist-plugin-objects/AFiberShader.h>
#include <gkg-core-exception/Exception.h>


std::string gkg::shaderCodeProfile( const std::string& coordinatesCountString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " uniform int isShaderedObject; ";
    shaderCode += " uniform int pointCount; ";
    shaderCode += " uniform vec3 coordinates[ ";
    shaderCode += coordinatesCountString;
    shaderCode += " ]; ";
    shaderCode += "uniform vec3 dataValue[ ";
    shaderCode += coordinatesCountString;
    shaderCode += " ];                                                         \
                                                                               \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedObject == 1 )                                           \
        {                                                                      \
                                                                               \
          int index = int( gl_Vertex.x );                                      \
          if( pointCount > index )                                             \
          {                                                                    \
                                                                               \
            gl_FrontColor =  vec4( dataValue[ index ].x,                       \
                                   dataValue[ index ].y,                       \
                                   dataValue[ index ].z,                       \
                                   0.2 );                                      \
            gl_Position.x =  coordinates[ index ].x;                           \
            gl_Position.y =  coordinates[ index ].y;                           \
            gl_Position.z =  coordinates[ index ].z;                           \
            gl_Position.w =  gl_Vertex.w;                                      \
            gl_Position = gl_ModelViewProjectionMatrix * ( gl_Position ) ;     \
                                                                               \
                                                                               \
          }                                                                    \
          else                                                                 \
          {                                                                    \
                                                                               \
            gl_FrontColor =  vec4( dataValue[ pointCount - 1 ].x,              \
                                   dataValue[ pointCount - 1 ].y,              \
                                   dataValue[ pointCount - 1 ].z,              \
                                   0.2 );                                      \
            gl_Position.x =  coordinates[ pointCount - 1 ].x;                  \
            gl_Position.y =  coordinates[ pointCount - 1 ].y;                  \
            gl_Position.z =  coordinates[ pointCount - 1 ].z;                  \
            gl_Position.w =  gl_Vertex.w;                                      \
            gl_Position = gl_ModelViewProjectionMatrix * ( gl_Position ) ;     \
                                                                               \
          }                                                                    \
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
  GKG_CATCH( "std::string gkg::shaderCode( "
             "const std::string& coordinatesCountString )" );

}


std::string gkg::shaderCodeBundle( const std::string& coordinatesCountString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " uniform int isShaderedObject; ";
    shaderCode += " uniform int pointCount; ";
    shaderCode += " uniform vec3 coordinates[ ";
    shaderCode += coordinatesCountString;
    shaderCode += " ]; ";
    shaderCode += "uniform vec3 dataValue;                                     \
                                                                               \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedObject == 1 )                                           \
        {                                                                      \
                                                                               \
          int index = int( gl_Vertex.x );                                      \
          if( pointCount > index )                                             \
          {                                                                    \
                                                                               \
            gl_FrontColor =  vec4( dataValue.x,                                \
                                   dataValue.y,                                \
                                   dataValue.z,                                \
                                   0.2 );                                      \
            gl_Position.x =  coordinates[ index ].x;                           \
            gl_Position.y =  coordinates[ index ].y;                           \
            gl_Position.z =  coordinates[ index ].z;                           \
            gl_Position.w =  gl_Vertex.w;                                      \
            gl_Position = gl_ModelViewProjectionMatrix * ( gl_Position ) ;     \
                                                                               \
                                                                               \
          }                                                                    \
          else                                                                 \
          {                                                                    \
                                                                               \
            gl_FrontColor =  vec4( dataValue.x,                                \
                                   dataValue.y,                                \
                                   dataValue.z,                                \
                                   0.2 );                                      \
            gl_Position.x =  coordinates[ pointCount - 1 ].x;                  \
            gl_Position.y =  coordinates[ pointCount - 1 ].y;                  \
            gl_Position.z =  coordinates[ pointCount - 1 ].z;                  \
            gl_Position.w =  gl_Vertex.w;                                      \
            gl_Position = gl_ModelViewProjectionMatrix * ( gl_Position ) ;     \
                                                                               \
          }                                                                    \
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
  GKG_CATCH( "std::string gkg::shaderCodeBundle( "
             "const std::string& coordinatesCountString )" );

}
