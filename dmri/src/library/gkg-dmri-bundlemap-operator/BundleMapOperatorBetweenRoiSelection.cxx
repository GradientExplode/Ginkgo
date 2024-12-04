#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBetweenRoiSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorBetweenRoiSelection< std::string >;
template class gkg::BundleMapOperatorBetweenRoiSelection< int16_t >;


static bool init_gkg_BundleMapOperatorBetweenRoiSelection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorBetweenRoiSelection< std::string,
                                             std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorBetweenRoiSelection< std::string,
                                              std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorBetweenRoiSelection< std::string,
                                             std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorBetweenRoiSelection< std::string,
                                             std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorBetweenRoiSelection_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorBetweenRoiSelection< int16_t,
                                             std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorBetweenRoiSelection< int16_t,
                                              std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorBetweenRoiSelection< int16_t,
                                             std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorBetweenRoiSelection< int16_t,
                                             std::less< int16_t > >::
                                                     getScalarParameterHelp() );
