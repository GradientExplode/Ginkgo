#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>



template class gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< std::string >;
template class gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< int16_t >;


static bool 
init_gkg_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_std_string 
                                                       __attribute__((unused)) =
 gkg::BundleMapOperatorFactory< std::string, std::less< std::string > >::
                                       getInstance().registerBundleMapOperator(
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< std::string,
                                        std::less< std::string > >::
                                                               getStaticName(),
 &gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< std::string,
                                         std::less< std::string > >::
                                                                createInstance,
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< std::string,
                                        std::less< std::string > >::
                                                    getStringParameterHelp(),
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< std::string,
                                        std::less< std::string > >::
                                                    getScalarParameterHelp() );

static bool 
init_gkg_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_int16_t 
                                                       __attribute__((unused)) =
 gkg::BundleMapOperatorFactory< int16_t, std::less< int16_t > >::getInstance().
                                                     registerBundleMapOperator(
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< int16_t,
                                        std::less< int16_t > >::
                                                               getStaticName(),
 &gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< int16_t,
                                         std::less< int16_t > >::
                                                                createInstance,
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< int16_t,
                                        std::less< int16_t > >::
                                                    getStringParameterHelp(),
 gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< int16_t,
                                        std::less< int16_t > >::
                                                    getScalarParameterHelp() );
