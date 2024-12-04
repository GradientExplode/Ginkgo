#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMaskIntersection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorMaskIntersection< std::string >;
template class gkg::BundleMapOperatorMaskIntersection< int16_t >;


static bool init_gkg_BundleMapOperatorMaskIntersection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorMaskIntersection< std::string,
                                           std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMaskIntersection< std::string,
                                            std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMaskIntersection< std::string,
                                           std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMaskIntersection< std::string,
                                           std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorMaskIntersection_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorMaskIntersection< int16_t,
                                           std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorMaskIntersection< int16_t,
                                            std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorMaskIntersection< int16_t,
                                           std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorMaskIntersection< int16_t,
                                           std::less< int16_t > >::
                                                     getScalarParameterHelp() );
