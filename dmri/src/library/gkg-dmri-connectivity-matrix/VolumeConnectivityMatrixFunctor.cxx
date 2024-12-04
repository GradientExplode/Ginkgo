#include <gkg-dmri-connectivity-matrix/VolumeConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template class gkg::VolumeConnectivityMatrixFunctor< int16_t,
                                                      std::less< int16_t > >;
template class gkg::VolumeConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool register_gkg_VolumeConnectivityMatrixFunctor_int16_t 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "none",
              gkg::VolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::VolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::VolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::VolumeConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool register_gkg_VolumeConnectivityMatrixFunctor_std_string 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "volume",
              "none",
              gkg::VolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::VolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::VolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::VolumeConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
