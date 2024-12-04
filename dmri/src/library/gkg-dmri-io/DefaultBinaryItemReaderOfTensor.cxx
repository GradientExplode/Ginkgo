#include <gkg-dmri-io/DefaultBinaryItemReaderOfTensor_i.h>


template class gkg::DefaultBinaryItemReader< gkg::Tensor >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                       gkg::Tensor >,
                             singleton );

