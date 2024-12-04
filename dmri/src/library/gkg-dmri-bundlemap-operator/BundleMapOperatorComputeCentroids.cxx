#include <gkg-dmri-bundlemap-operator/BundleMapOperatorComputeCentroids_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorComputeCentroids< std::string >;
template class gkg::BundleMapOperatorComputeCentroids< int16_t >;


static bool init_gkg_BundleMapOperatorComputeCentroids_std_string
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorComputeCentroids< std::string,
                                        std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorComputeCentroids< std::string,
                                         std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorComputeCentroids< std::string,
                                        std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorComputeCentroids< std::string,
                                        std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorComputeCentroids_int16_t
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorComputeCentroids< int16_t,
                                        std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorComputeCentroids< int16_t,
                                         std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorComputeCentroids< int16_t,
                                        std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorComputeCentroids< int16_t,
                                        std::less< int16_t > >::
                                                     getScalarParameterHelp() );
