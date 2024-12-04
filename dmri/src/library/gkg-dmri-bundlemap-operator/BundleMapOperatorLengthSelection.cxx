#include <gkg-dmri-bundlemap-operator/BundleMapOperatorLengthSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorLengthSelection< std::string >;
template class gkg::BundleMapOperatorLengthSelection< int16_t >;


static bool init_gkg_BundleMapOperatorLengthSelection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorLengthSelection< std::string,
                                     std::less< std::string > >::
                                                             getStaticName(),
  &gkg::BundleMapOperatorLengthSelection< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorLengthSelection< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorLengthSelection< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorLengthSelection_int16_t  
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorLengthSelection< int16_t,
                                     std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorLengthSelection< int16_t,
                                      std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorLengthSelection< int16_t,
                                     std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorLengthSelection< int16_t,
                                     std::less< int16_t > >::
                                                     getScalarParameterHelp() );
