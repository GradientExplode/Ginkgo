#include <gkg-dmri-tractography/DeterministicTractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>



template class gkg::DeterministicTractographyAlgorithm< std::string >;
template class gkg::DeterministicTractographyAlgorithm< int16_t >;
template class gkg::DeterministicTractographyAlgorithm< int32_t >;


static bool init_gkg_DeterministicTractographyAlgorithm_std_string
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< std::string >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::DeterministicTractographyAlgorithm< std::string >::getStaticName(),
  &gkg::DeterministicTractographyAlgorithm< std::string >::createInstance );

static bool init_gkg_DeterministicTractographyAlgorithm_int16_t
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int16_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::DeterministicTractographyAlgorithm< int16_t >::getStaticName(),
  &gkg::DeterministicTractographyAlgorithm< int16_t >::createInstance );

static bool init_gkg_DeterministicTractographyAlgorithm_int32_t
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int32_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::DeterministicTractographyAlgorithm< int32_t >::getStaticName(),
  &gkg::DeterministicTractographyAlgorithm< int32_t >::createInstance );
