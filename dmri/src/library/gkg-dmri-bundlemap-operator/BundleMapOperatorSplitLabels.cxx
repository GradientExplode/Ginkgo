#include <gkg-dmri-bundlemap-operator/BundleMapOperatorSplitLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorSplitLabels< std::string >;
template class gkg::BundleMapOperatorSplitLabels< int16_t >;


static bool init_gkg_BundleMapOperatorSplitLabels_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorSplitLabels< std::string,
                                     std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorSplitLabels< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorSplitLabels< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorSplitLabels< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorSplitLabels_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorSplitLabels< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorSplitLabels< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorSplitLabels< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorSplitLabels< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
