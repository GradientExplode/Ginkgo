#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFlipper3d_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorFlipper3d< std::string >;
template class gkg::BundleMapOperatorFlipper3d< int16_t >;


static bool init_gkg_BundleMapOperatorFlipper3d_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorFlipper3d< std::string,
                                   std::less< std::string > >::
                                                             getStaticName(),
  &gkg::BundleMapOperatorFlipper3d< std::string,
                                    std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFlipper3d< std::string,
                                   std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFlipper3d< std::string,
                                   std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorFlipper3d_int16_t
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorFlipper3d< int16_t,
                                   std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorFlipper3d< int16_t,
                                    std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorFlipper3d< int16_t,
                                   std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorFlipper3d< int16_t,
                                   std::less< int16_t > >::
                                                     getScalarParameterHelp() );
