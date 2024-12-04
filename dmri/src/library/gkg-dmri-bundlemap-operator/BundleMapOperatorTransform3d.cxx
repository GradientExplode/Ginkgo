#include <gkg-dmri-bundlemap-operator/BundleMapOperatorTransform3d_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorTransform3d< std::string >;
template class gkg::BundleMapOperatorTransform3d< int16_t >;


static bool init_gkg_BundleMapOperatorTransform3d_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorTransform3d< std::string,
                                     std::less< std::string > >::
                                                             getStaticName(),
  &gkg::BundleMapOperatorTransform3d< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorTransform3d< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorTransform3d< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorTransform3d_int16_t  
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorTransform3d< int16_t,
                                     std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorTransform3d< int16_t,
                                      std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorTransform3d< int16_t,
                                     std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorTransform3d< int16_t,
                                     std::less< int16_t > >::
                                                     getScalarParameterHelp() );
