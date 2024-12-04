#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfEap_i.h>


ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       gkg::OrientationDistributionFunction >,
                             singleton );
