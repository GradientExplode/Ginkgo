#include <gkg-dmri-tractography/RegularizedDeterministicTractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>



template class gkg::RegularizedDeterministicTractographyAlgorithm<
                                                                  std::string >;
template class gkg::RegularizedDeterministicTractographyAlgorithm< int16_t >;


static bool init_gkg_RegularizedDeterministicTractographyAlgorithm_std_string
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< std::string >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::RegularizedDeterministicTractographyAlgorithm< std::string >::
                                                                getStaticName(),
  &gkg::RegularizedDeterministicTractographyAlgorithm< std::string >::
                                                               createInstance );

static bool init_gkg_RegularizedDeterministicTractographyAlgorithm_int16_t
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int16_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::RegularizedDeterministicTractographyAlgorithm< int16_t >::
                                                               getStaticName(),
  &gkg::RegularizedDeterministicTractographyAlgorithm< int16_t >::
                                                               createInstance );
