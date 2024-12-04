#include <gkg-dmri-plugin-functors/MpiTest/GfaAlgorithm_i.h>
#include <gkg-dmri-plugin-functors/MpiTest/ParallelizedAlgorithmFactory.h>


static bool init_gkg_GfaAlgorithm
                                                       __attribute__((unused)) =
  gkg::ParallelizedAlgorithmFactory::getInstance().registerParallelizedAlgorithm(
  gkg::GfaAlgorithm::getStaticName(),
  &gkg::GfaAlgorithm::createInstance );
