#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRoiBasedSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorRoiBasedSelection< std::string >;
template class gkg::BundleMapOperatorRoiBasedSelection< int16_t >;


static bool init_gkg_BundleMapOperatorRoiBasedSelection_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorRoiBasedSelection< std::string,
                                           std::less< std::string > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorRoiBasedSelection< std::string,
                                            std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRoiBasedSelection< std::string,
                                           std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRoiBasedSelection< std::string,
                                           std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorRoiBasedSelection_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorRoiBasedSelection< int16_t,
                                           std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorRoiBasedSelection< int16_t,
                                            std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRoiBasedSelection< int16_t,
                                           std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRoiBasedSelection< int16_t,
                                           std::less< int16_t > >::
                                                     getScalarParameterHelp() );
