#include <gkg-anatomist-plugin-objects/VertexShader.h>
#include <gkg-core-exception/Exception.h>


std::string gkg::shaderCode( const std::string& outputOrientationCountString,
                             const std::string& minimumResolutionString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " attribute float meshScale; ";
    shaderCode += " uniform int isShaderedObject; ";
    shaderCode += " uniform vec4 translation; ";
    shaderCode += "uniform vec3 normal[ ";
    shaderCode += outputOrientationCountString;
    shaderCode += " ] ; ";
    shaderCode += "uniform float transformation[ ";
    shaderCode += outputOrientationCountString;
    shaderCode += " ];                                                         \
      attribute float index;                                                   \
                                                                               \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedObject == 1 )                                           \
        {";
    shaderCode += " float minimumResolution = float( " +                       \
                                    minimumResolutionString + " ) * meshScale; \
                                                                               \
          vec4 v = vec4( gl_Vertex.x , gl_Vertex.y, gl_Vertex.z, 1.0 );        \
          if ( v.x < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.x = abs( v.x );                                                  \
                                                                               \
          }                                                                    \
          if ( v.y < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.y = abs( v.y );                                                  \
                                                                               \
          }                                                                    \
          if (v.z < 0.0 )                                                      \
          {                                                                    \
                                                                               \
            v.z = abs( v.z );                                                  \
                                                                               \
          }                                                                    \
                                                                               \
          vec3 norm = normal[ int( index ) ];                                  \
          vec3 normalNormalized, lightDir;                                     \
          float NdotL;                                                         \
          normalNormalized = gl_NormalMatrix * norm;                           \
          lightDir = vec3( gl_LightSource[ 0 ].position );                     \
          NdotL = dot( normalNormalized, lightDir );                           \
          gl_FrontColor =  NdotL * v;                                          \
                                                                               \
          gl_Position.x =  transformation[ int( index ) ] * gl_Vertex.x        \
                                                          * minimumResolution; \
          gl_Position.y =  transformation[ int( index ) ] * gl_Vertex.y        \
                                                          * minimumResolution; \
          gl_Position.z =  transformation[ int( index ) ] * gl_Vertex.z        \
                                                          * minimumResolution; \
          gl_Position.w =  gl_Vertex.w;                                        \
                                                                               \
          gl_Position = gl_ModelViewProjectionMatrix *                         \
                                                ( gl_Position + translation ) ;\
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
             "const std::string& outputOrientationCountString, "
             "const std::string& minimumResolutionString ) " );

}


std::string gkg::shaderCodeWithApproximatedNormals(
                               const std::string& outputOrientationCountString,
                               const std::string& minimumResolutionString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " attribute float meshScale; ";
    shaderCode += " uniform int isShaderedObject; ";
    shaderCode += " uniform vec4 translation; ";
    shaderCode += "uniform float transformation[ " ;
    shaderCode += outputOrientationCountString;
    shaderCode += " ];                                                         \
      attribute float index;                                                   \
      attribute vec3 coordinates2;                                             \
      attribute vec3 coordinates3;                                             \
      attribute vec2 polygonIndices;                                           \
                                                                               \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedObject == 1 )                                           \
        {";
    shaderCode += " float minimumResolution = float( " +                       \
                                                minimumResolutionString + " ); \
                                                                               \
          vec4 v = vec4( gl_Vertex.x , gl_Vertex.y, gl_Vertex.z, 1.0 );        \
          if ( v.x < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.x = abs( v.x );                                                  \
                                                                               \
          }                                                                    \
          if ( v.y < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.y = abs( v.y );                                                  \
                                                                               \
          }                                                                    \
          if ( v.z < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.z = abs( v.z );                                                  \
                                                                               \
          }                                                                    \
                                                                               \
          vec3 V1;                                                             \
          V1.x =  transformation[ int( index ) ] * gl_Vertex.x;                \
          V1.y =  transformation[ int( index ) ] * gl_Vertex.y;                \
          V1.z =  transformation[ int( index ) ] * gl_Vertex.z;                \
                                                                               \
          vec3 V2;                                                             \
          vec3 V3;                                                             \
          int polygonIndexX = int( polygonIndices.x );                         \
          int polygonIndexY = int( polygonIndices.y );                         \
          V2.x = ( transformation[ polygonIndexX ] * coordinates2.x ) - V1.x;  \
          V2.y = ( transformation[ polygonIndexX ] * coordinates2.y ) - V1.y;  \
          V2.z = ( transformation[ polygonIndexX ] * coordinates2.z ) - V1.z;  \
          V3.x =   ( transformation[ polygonIndexY ] * coordinates3.x ) - V1.x;\
          V3.y =   ( transformation[ polygonIndexY ] * coordinates3.y ) - V1.y;\
          V3.z =   ( transformation[ polygonIndexY ] * coordinates3.z ) - V1.z;\
                                                                               \
          vec3 norm = normalize( cross( V2, V3 ) );                            \
          vec3 normalNormalized, lightDir;                                     \
          float NormDotLight;                                                  \
          normalNormalized = gl_NormalMatrix * norm;                           \
          lightDir = vec3( gl_LightSource[ 0 ].position );                     \
          NormDotLight = dot( normalNormalized, lightDir );                    \
          gl_FrontColor =  NormDotLight * v;                                   \
                                                                               \
          gl_Position.x =  V1.x * meshScale * minimumResolution;               \
          gl_Position.y =  V1.y * meshScale * minimumResolution;               \
          gl_Position.z =  V1.z * meshScale * minimumResolution;               \
          gl_Position.w =  gl_Vertex.w;                                        \
                                                                               \
          gl_Position = gl_ModelViewProjectionMatrix *                         \
                                                ( gl_Position +  translation );\
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
      }";

    return shaderCode;

  }
  GKG_CATCH( "std::string gkg::shaderCodeWithApproximatedNormals("
             "const std::string& outputOrientationCountString, "
             "const std::string& minimumResolutionString )" );

}


std::string gkg::shaderCodeWithoutNormals( 
                               const std::string& outputOrientationCountString,
                               const std::string& minimumResolutionString )
{

  try
  {

    std::string shaderCode;
    shaderCode += " attribute float meshScale; ";
    shaderCode += " uniform int isShaderedObject; ";
    shaderCode += " uniform vec4 translation; ";
    shaderCode += "uniform float transformation[ " ;
    shaderCode += outputOrientationCountString;
    shaderCode += " ];                                                         \
      attribute float index;                                                   \
                                                                               \
      void main(void)                                                          \
      {                                                                        \
                                                                               \
        if ( isShaderedObject == 1 )                                           \
        {";
    shaderCode += " float minimumResolution = float( " +                       \
                                                minimumResolutionString + " ); \
                                                                               \
          vec4 v = vec4( gl_Vertex.x , gl_Vertex.y, gl_Vertex.z, 1.0 );        \
          if ( v.x < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.x = abs( v.x );                                                  \
                                                                               \
          }                                                                    \
          if ( v.y < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.y = abs( v.y );                                                  \
                                                                               \
          }                                                                    \
          if ( v.z < 0.0 )                                                     \
          {                                                                    \
                                                                               \
            v.z = abs( v.z );                                                  \
                                                                               \
          }                                                                    \
                                                                               \
          gl_FrontColor =   v;                                                 \
                                                                               \
          gl_Position.x = transformation[ int( index ) ] * gl_Vertex.x         \
                                                         * meshScale           \
                                                         * minimumResolution;  \
          gl_Position.y = transformation[ int( index ) ] * gl_Vertex.y         \
                                                         * meshScale           \
                                                         * minimumResolution;  \
          gl_Position.z =  transformation[ int( index ) ] * gl_Vertex.z        \
                                                          * meshScale          \
                                                          * minimumResolution; \
          gl_Position.w =  gl_Vertex.w;                                        \
                                                                               \
          gl_Position = gl_ModelViewProjectionMatrix * \
                                                ( gl_Position +  translation );\
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
      }";

    return shaderCode;

  }
  GKG_CATCH( "std::string shaderCodeWithoutNormals("
             "const std::string& outputOrientationCountString, "
             "const std::string& minimumResolutionString )" );

}
