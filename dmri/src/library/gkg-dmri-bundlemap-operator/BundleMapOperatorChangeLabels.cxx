#include <gkg-dmri-bundlemap-operator/BundleMapOperatorChangeLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorChangeLabels< std::string >;
template class gkg::BundleMapOperatorChangeLabels< int16_t >;


static bool init_gkg_BundleMapOperatorChangeLabels_std_string
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorChangeLabels< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorChangeLabels< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorChangeLabels< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorChangeLabels< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorChangeLabels_int16_t
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorChangeLabels< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorChangeLabels< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorChangeLabels< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorChangeLabels< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
