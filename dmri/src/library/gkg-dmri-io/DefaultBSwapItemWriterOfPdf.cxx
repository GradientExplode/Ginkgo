#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>


template class gkg::DefaultBSwapItemWriter<
                                         gkg::ProbabilityDensityFunction >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       gkg::ProbabilityDensityFunction >,
                             singleton );
