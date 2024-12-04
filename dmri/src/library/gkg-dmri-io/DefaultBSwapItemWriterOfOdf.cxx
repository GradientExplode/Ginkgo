#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>


template class gkg::DefaultBSwapItemWriter<
                                         gkg::OrientationDistributionFunction >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       gkg::OrientationDistributionFunction >,
                             singleton );
