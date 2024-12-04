#include <gkg-dmri-connectivity-matrix/MeshToMeshAndTextureConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template class gkg::MeshToMeshAndTextureConnectivityMatrixFunctor< int16_t,
                                                      std::less< int16_t > >;
template class gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool register_gkg_MeshToMeshAndTextureConnectivityMatrixFunctor_int16_t 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh",
              "mesh-and-texture",
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool 
register_gkg_MeshToMeshAndTextureConnectivityMatrixFunctor_std_string 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh",
              "mesh-and-texture",
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::MeshToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
