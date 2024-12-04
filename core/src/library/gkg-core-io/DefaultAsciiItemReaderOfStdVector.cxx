#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector_i.h>


template class gkg::DefaultAsciiItemReader< std::vector< uint8_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< int8_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< uint16_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< int16_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< uint32_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< int32_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< uint64_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< int64_t > >;
template class gkg::DefaultAsciiItemReader< std::vector< float > >;
template class gkg::DefaultAsciiItemReader< std::vector< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< uint8_t > >,
                             singleton_std_vector_uint8_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< int8_t > >,
                             singleton_std_vector_int8_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< uint16_t > >,
                             singleton_std_vector_uint16_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< int16_t > >,
                             singleton_std_vector_int16_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< uint32_t > >,
                             singleton_std_vector_uint32_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< int32_t > >,
                             singleton_std_vector_int32_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< uint64_t > >,
                             singleton_std_vector_uint64_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< int64_t > >,
                             singleton_std_vector_int64_t );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< float > >,
                             singleton_std_vector_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                       std::vector< double > >,
                             singleton_std_vector_double );
