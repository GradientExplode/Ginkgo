#include <gkg-dmri-io/DefaultAsciiItemReaderOfTensor_i.h>


template class gkg::DefaultAsciiItemReader< gkg::Tensor >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       gkg::Tensor >,
                             singleton );

