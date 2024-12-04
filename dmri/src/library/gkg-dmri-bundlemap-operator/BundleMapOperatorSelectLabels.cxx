#include <gkg-dmri-bundlemap-operator/BundleMapOperatorSelectLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorSelectLabels< std::string >;
template class gkg::BundleMapOperatorSelectLabels< int16_t >;


static bool init_gkg_BundleMapOperatorSelectLabels_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorSelectLabels< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorSelectLabels< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorSelectLabels< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorSelectLabels< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorSelectLabels_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
