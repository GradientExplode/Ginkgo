#include <gkg-dmri-connectivity-matrix/MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template 
class gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                                                     int16_t,
                                                     std::less< int16_t > >;
template
class gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool 
register_gkg_MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor_int16_t 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh-and-texture",
              "mesh-and-texture",
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool 
register_gkg_MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor_std_string
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh-and-texture",
              "mesh-and-texture",
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::MeshAndTextureToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
