#include <gkg-dmri-io/DefaultAsciiItemWriterOfTensor_i.h>


template class gkg::DefaultAsciiItemWriter< gkg::Tensor >;
				       
ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                       gkg::Tensor >,
                             singleton );

