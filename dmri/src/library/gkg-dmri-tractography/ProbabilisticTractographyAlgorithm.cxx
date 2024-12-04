#include <gkg-dmri-tractography/ProbabilisticTractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>



template class gkg::ProbabilisticTractographyAlgorithm< std::string >;
template class gkg::ProbabilisticTractographyAlgorithm< int16_t >;


static bool init_gkg_ProbabilisticTractographyAlgorithm_std_string
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< std::string >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::ProbabilisticTractographyAlgorithm< std::string >::getStaticName(),
  &gkg::ProbabilisticTractographyAlgorithm< std::string >::createInstance );

static bool init_gkg_ProbabilisticTractographyAlgorithm_int16_t 
                                                       __attribute__((unused)) =
  gkg::TractographyAlgorithmFactory< int16_t >::getInstance().
                                                  registerTractographyAlgorithm(
  gkg::ProbabilisticTractographyAlgorithm< int16_t >::getStaticName(),
  &gkg::ProbabilisticTractographyAlgorithm< int16_t >::createInstance );
