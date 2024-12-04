#include <gkg-dmri-bundlemap-operator/BundleMapOperatorLengthNormalization_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorLengthNormalization< std::string >;
template class gkg::BundleMapOperatorLengthNormalization< int16_t >;


static bool init_gkg_BundleMapOperatorLengthNormalization_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorLengthNormalization< std::string,
                                     std::less< std::string > >::
                                                             getStaticName(),
  &gkg::BundleMapOperatorLengthNormalization< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorLengthNormalization< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorLengthNormalization< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorLengthNormalization_int16_t  
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorLengthNormalization< int16_t,
                                     std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorLengthNormalization< int16_t,
                                      std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorLengthNormalization< int16_t,
                                     std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorLengthNormalization< int16_t,
                                     std::less< int16_t > >::
                                                     getScalarParameterHelp() );
