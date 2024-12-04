#include <gkg-dmri-bundlemap-operator/BundleMapOperatorClusteringSelectLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorClusteringSelectLabels< std::string >;
template class gkg::BundleMapOperatorClusteringSelectLabels< int16_t >;


static bool init_gkg_BundleMapOperatorClusteringSelectLabels_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorClusteringSelectLabels< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorClusteringSelectLabels< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorClusteringSelectLabels< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorClusteringSelectLabels< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorClusteringSelectLabels_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorClusteringSelectLabels< int16_t,
                                                 std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorClusteringSelectLabels< int16_t,
                                                 std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorClusteringSelectLabels< int16_t,
                                                std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorClusteringSelectLabels< int16_t,
                                                std::less< int16_t > >::
                                                     getScalarParameterHelp() );
