#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector_i.h>


template class gkg::DefaultBSwapItemWriter< std::vector< uint8_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< int8_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< uint16_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< int16_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< uint32_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< int32_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< uint64_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< int64_t > >;
template class gkg::DefaultBSwapItemWriter< std::vector< float > >;
template class gkg::DefaultBSwapItemWriter< std::vector< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< uint8_t > >,
                             singleton_std_vector_uint8_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< int8_t > >,
                             singleton_std_vector_int8_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< uint16_t > >,
                             singleton_std_vector_uint16_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< int16_t > >,
                             singleton_std_vector_int16_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< uint32_t > >,
                             singleton_std_vector_uint32_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< int32_t > >,
                             singleton_std_vector_int32_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< uint64_t > >,
                             singleton_std_vector_uint64_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< int64_t > >,
                             singleton_std_vector_int64_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< float > >,
                             singleton_std_vector_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::vector< double > >,
                             singleton_std_vector_double );
