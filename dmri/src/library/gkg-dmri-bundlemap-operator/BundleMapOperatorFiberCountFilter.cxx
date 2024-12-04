#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFiberCountFilter_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorFiberCountFilter< std::string >;
template class gkg::BundleMapOperatorFiberCountFilter< int16_t >;


static bool init_gkg_BundleMapOperatorFiberCountFilter_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorFiberCountFilter< std::string,
                                            std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorFiberCountFilter< std::string,
                                             std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFiberCountFilter< std::string,
                                            std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFiberCountFilter< std::string,
                                            std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorFiberCountFilter_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorFiberCountFilter< int16_t, std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorFiberCountFilter< int16_t, std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFiberCountFilter< int16_t, std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFiberCountFilter< int16_t, std::less< int16_t > >::
                                                     getScalarParameterHelp() );
