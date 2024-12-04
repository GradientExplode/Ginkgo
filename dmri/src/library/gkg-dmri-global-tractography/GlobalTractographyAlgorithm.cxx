#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>


template class gkg::GlobalTractographyAlgorithm< std::string >;
template class gkg::GlobalTractographyAlgorithm< int16_t >;
template class gkg::GlobalTractographyAlgorithm< int32_t >;


static bool init_gkg_GlobalTractographyAlgorithm_std_string 
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< std::string >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::GlobalTractographyAlgorithm< std::string >::getStaticName(),
  &gkg::GlobalTractographyAlgorithm< std::string >::createInstance );

static bool init_gkg_GlobalTractographyAlgorithm_int16_t
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int16_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::GlobalTractographyAlgorithm< int16_t >::getStaticName(),
  &gkg::GlobalTractographyAlgorithm< int16_t >::createInstance );

static bool init_gkg_GlobalTractographyAlgorithm_int32_t
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int32_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::GlobalTractographyAlgorithm< int32_t >::getStaticName(),
  &gkg::GlobalTractographyAlgorithm< int32_t >::createInstance );

