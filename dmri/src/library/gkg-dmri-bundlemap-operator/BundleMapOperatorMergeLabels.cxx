#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMergeLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorMergeLabels< std::string >;
template class gkg::BundleMapOperatorMergeLabels< int16_t >;


static bool init_gkg_BundleMapOperatorMergeLabels_std_string
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorMergeLabels< std::string,
                                     std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMergeLabels< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMergeLabels< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMergeLabels< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorMergeLabels_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorMergeLabels< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMergeLabels< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMergeLabels< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMergeLabels< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
