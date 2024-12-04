#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusoid_i.h>


template class gkg::DefaultBSwapItemWriter< gkg::Diffusoid >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter< gkg::Diffusoid >,
                             singleton );
