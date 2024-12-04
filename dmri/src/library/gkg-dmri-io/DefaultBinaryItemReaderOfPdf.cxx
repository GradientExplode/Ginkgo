#include <gkg-dmri-io/DefaultBinaryItemReaderOfPdf_i.h>


template class gkg::DefaultBinaryItemReader<
                                         gkg::ProbabilityDensityFunction >;
				       
ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                       gkg::ProbabilityDensityFunction >,
                             singleton );

