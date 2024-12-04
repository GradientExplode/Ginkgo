#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFusion_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorFusion< std::string >;
template class gkg::BundleMapOperatorFusion< int16_t >;


static bool init_gkg_BundleMapOperatorFusion_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorFusion< std::string, std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorFusion< std::string, std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFusion< std::string, std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFusion< std::string, std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorFusion_int16_t __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorFusion< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorFusion< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFusion< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFusion< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
