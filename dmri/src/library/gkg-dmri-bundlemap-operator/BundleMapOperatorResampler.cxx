#include <gkg-dmri-bundlemap-operator/BundleMapOperatorResampler_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorResampler< std::string >;
template class gkg::BundleMapOperatorResampler< int16_t >;


static bool init_gkg_BundleMapOperatorResampler_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorResampler< std::string,
                                   std::less< std::string > >::getStaticName(),
  &gkg::BundleMapOperatorResampler< std::string,
                                    std::less< std::string > >::createInstance,
  gkg::BundleMapOperatorResampler< std::string,
                                   std::less< std::string > >::
                                                       getStringParameterHelp(),
  gkg::BundleMapOperatorResampler< std::string,
                                   std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorResampler_int16_t 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorResampler< int16_t,
                                   std::less< int16_t > >::getStaticName(),
  &gkg::BundleMapOperatorResampler< int16_t,
                                    std::less< int16_t > >::createInstance,
  gkg::BundleMapOperatorResampler< int16_t,
                                   std::less< int16_t > >::
                                                       getStringParameterHelp(),
  gkg::BundleMapOperatorResampler< int16_t,
                                   std::less< int16_t > >::
                                                     getScalarParameterHelp() );
