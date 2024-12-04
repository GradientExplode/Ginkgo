#include <gkg-dmri-io/DefaultBinaryItemWriterOfPdf_i.h>


template class gkg::DefaultBinaryItemWriter<
                                         gkg::ProbabilityDensityFunction >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                       gkg::ProbabilityDensityFunction >,
                             singleton );
