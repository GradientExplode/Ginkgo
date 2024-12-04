#include <gkg-dmri-bundlemap-operator/BundleMapOperatorIntraSubjectClusterLabelSimplifier_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string >;
template class gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t >;


static bool 
init_gkg_BundleMapOperatorIntraSubjectClusterLabelSimplifier_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool 
init_gkg_BundleMapOperatorIntraSubjectClusterLabelSimplifier_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t,
                                                 std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t,
                                                 std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t,
                                                std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t,
                                                std::less< int16_t > >::
                                                     getScalarParameterHelp() );
