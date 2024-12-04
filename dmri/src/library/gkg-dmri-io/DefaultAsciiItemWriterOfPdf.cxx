#include <gkg-dmri-io/DefaultAsciiItemWriterOfPdf_i.h>


template class gkg::DefaultAsciiItemWriter<
                                         gkg::ProbabilityDensityFunction >;
				       
ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                       gkg::ProbabilityDensityFunction >,
                             singleton );

