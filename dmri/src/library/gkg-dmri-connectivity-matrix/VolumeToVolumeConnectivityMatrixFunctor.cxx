#include <gkg-dmri-connectivity-matrix/VolumeToVolumeConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template class gkg::VolumeToVolumeConnectivityMatrixFunctor< int16_t,
                                                      std::less< int16_t > >;
template class gkg::VolumeToVolumeConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool register_gkg_VolumeToVolumeConnectivityMatrixFunctor_int16_t 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "volume",
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool register_gkg_VolumeToVolumeConnectivityMatrixFunctor_std_string 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "volume",
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::VolumeToVolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
