#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRandomSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorRandomSelection< std::string >;
template class gkg::BundleMapOperatorRandomSelection< int16_t >;


static bool init_gkg_BundleMapOperatorRandomSelection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorRandomSelection< std::string,
                                         std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorRandomSelection< std::string,
                                          std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRandomSelection< std::string,
                                         std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRandomSelection< std::string,
                                         std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorRandomSelection_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorRandomSelection< int16_t,
                                         std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorRandomSelection< int16_t,
                                          std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRandomSelection< int16_t,
                                         std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRandomSelection< int16_t,
                                         std::less< int16_t > >::
                                                     getScalarParameterHelp() );
