#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBetweenRoiPairSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorBetweenRoiPairSelection< std::string >;
template class gkg::BundleMapOperatorBetweenRoiPairSelection< int16_t >;


static bool init_gkg_BundleMapOperatorBetweenRoiPairSelection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorBetweenRoiPairSelection< std::string,
                                           std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorBetweenRoiPairSelection< std::string,
                                            std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorBetweenRoiPairSelection< std::string,
                                           std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorBetweenRoiPairSelection< std::string,
                                           std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorBetweenRoiPairSelection_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorBetweenRoiPairSelection< int16_t,
                                           std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorBetweenRoiPairSelection< int16_t,
                                            std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorBetweenRoiPairSelection< int16_t,
                                           std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorBetweenRoiPairSelection< int16_t,
                                           std::less< int16_t > >::
                                                     getScalarParameterHelp() );
