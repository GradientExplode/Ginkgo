#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusionFeature_i.h>


template class gkg::DefaultBSwapItemWriter< gkg::DiffusionFeature >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       gkg::DiffusionFeature >,
                             singleton );
