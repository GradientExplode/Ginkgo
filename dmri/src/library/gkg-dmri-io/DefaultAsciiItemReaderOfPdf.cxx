#include <gkg-dmri-io/DefaultAsciiItemReaderOfPdf_i.h>


template class gkg::DefaultAsciiItemReader<
                                         gkg::ProbabilityDensityFunction >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       gkg::ProbabilityDensityFunction >,
                             singleton );
