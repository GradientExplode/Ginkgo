#include <gkg-dmri-bundlemap-operator/BundleMapOperatorCutExtremities_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorCutExtremities< std::string >;
template class gkg::BundleMapOperatorCutExtremities< int16_t >;


static bool init_gkg_BundleMapOperatorCutExtremities_std_string
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorCutExtremities< std::string,
                                        std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorCutExtremities< std::string,
                                         std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorCutExtremities< std::string,
                                        std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorCutExtremities< std::string,
                                        std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorCutExtremities_int16_t
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorCutExtremities< int16_t,
                                        std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorCutExtremities< int16_t,
                                         std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorCutExtremities< int16_t,
                                        std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorCutExtremities< int16_t,
                                        std::less< int16_t > >::
                                                     getScalarParameterHelp() );
