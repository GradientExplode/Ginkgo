#include <gkg-dmri-connectivity-matrix/VolumeToMeshAndTextureConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template class gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor< int16_t,
                                                      std::less< int16_t > >;
template class gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool 
register_gkg_VolumeToMeshAndTextureConnectivityMatrixFunctor_int16_t
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "mesh-and-texture",
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool 
register_gkg_VolumeToMeshAndTextureConnectivityMatrixFunctor_std_string 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "mesh-and-texture",
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::VolumeToMeshAndTextureConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
