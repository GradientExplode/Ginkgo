#include <gkg-dmri-bundlemap-operator/BundleMapOperatorDiscardLabels_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorDiscardLabels< std::string >;
template class gkg::BundleMapOperatorDiscardLabels< int16_t >;


static bool init_gkg_BundleMapOperatorDiscardLabels_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorDiscardLabels< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorDiscardLabels< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorDiscardLabels< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorDiscardLabels< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorDiscardLabels_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorDiscardLabels< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorDiscardLabels< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorDiscardLabels< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorDiscardLabels< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
