#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMidSagittalPlaneSymmetrizer_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< std::string >;
template class gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< int16_t >;


static bool init_gkg_BundleMapOperatorMidSagittalPlaneSymmetrizer_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        std::string,
                                        std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        std::string,
                                        std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        std::string,
                                        std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorMidSagittalPlaneSymmetrizer_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        int16_t,
                                        std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        int16_t,
                                        std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        int16_t,
                                        std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                        int16_t,
                                        std::less< int16_t > >::
                                                     getScalarParameterHelp() );
