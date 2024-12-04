#include <gkg-dmri-bundlemap-operator/BundleMapOperatorReverseFibers_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorReverseFibers< std::string >;
template class gkg::BundleMapOperatorReverseFibers< int16_t >;


static bool init_gkg_BundleMapOperatorReverseFibers_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorReverseFibers< std::string,
                                       std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorReverseFibers< std::string,
                                        std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorReverseFibers< std::string,
                                       std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorReverseFibers< std::string,
                                       std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorReverseFibers_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorReverseFibers< int16_t,
                                       std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorReverseFibers< int16_t,
                                        std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorReverseFibers< int16_t,
                                       std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorReverseFibers< int16_t,
                                       std::less< int16_t > >::
                                                     getScalarParameterHelp() );
