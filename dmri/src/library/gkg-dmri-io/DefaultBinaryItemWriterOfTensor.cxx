#include <gkg-dmri-io/DefaultBinaryItemWriterOfTensor_i.h>


template class gkg::DefaultBinaryItemWriter< gkg::Tensor >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                       gkg::Tensor >,
                             singleton );

