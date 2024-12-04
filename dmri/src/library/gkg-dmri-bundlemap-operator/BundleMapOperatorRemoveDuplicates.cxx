#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRemoveDuplicates_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorRemoveDuplicates< std::string >;
template class gkg::BundleMapOperatorRemoveDuplicates< int16_t >;


static bool init_gkg_BundleMapOperatorRemoveDuplicates_std_string 
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                        getInstance().registerBundleMapOperator(
  gkg::BundleMapOperatorRemoveDuplicates< std::string,
                                     std::less< std::string > >::
                                                             getStaticName(),
  &gkg::BundleMapOperatorRemoveDuplicates< std::string,
                                      std::less< std::string > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRemoveDuplicates< std::string,
                                     std::less< std::string > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRemoveDuplicates< std::string,
                                     std::less< std::string > >::
                                                     getScalarParameterHelp() );

static bool init_gkg_BundleMapOperatorRemoveDuplicates_int16_t  
                                                       __attribute__((unused)) =
  gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                      registerBundleMapOperator(
  gkg::BundleMapOperatorRemoveDuplicates< int16_t,
                                     std::less< int16_t > >::
                                                                getStaticName(),
  &gkg::BundleMapOperatorRemoveDuplicates< int16_t,
                                      std::less< int16_t > >::
                                                                 createInstance,
  gkg::BundleMapOperatorRemoveDuplicates< int16_t,
                                     std::less< int16_t > >::
                                                     getStringParameterHelp(),
  gkg::BundleMapOperatorRemoveDuplicates< int16_t,
                                     std::less< int16_t > >::
                                                     getScalarParameterHelp() );
