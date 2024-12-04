#include <gkg-dmri-bundlemap-operator/BundleMapOperatorOrderExtremities_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorOrderExtremities< std::string >;
template class gkg::BundleMapOperatorOrderExtremities< int16_t >;


static bool init_gkg_BundleMapOperatorOrderExtremities_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorOrderExtremities< std::string,
                                          std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorOrderExtremities< std::string,
                                           std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorOrderExtremities< std::string,
                                          std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorOrderExtremities< std::string,
                                          std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorOrderExtremities_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorOrderExtremities< int16_t,
                                          std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorOrderExtremities< int16_t,
                                           std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorOrderExtremities< int16_t,
                                          std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorOrderExtremities< int16_t,
                                          std::less< int16_t > >::
                                                     getScalarParameterHelp() );
