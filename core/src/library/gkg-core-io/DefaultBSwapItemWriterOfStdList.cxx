#include <gkg-core-io/DefaultBSwapItemWriterOfStdList_i.h>


template class gkg::DefaultBSwapItemWriter< std::list< uint8_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< int8_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< uint16_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< int16_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< uint32_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< int32_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< uint64_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< int64_t > >;
template class gkg::DefaultBSwapItemWriter< std::list< float > >;
template class gkg::DefaultBSwapItemWriter< std::list< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< uint8_t > >,
                             singleton_std_list_uint8_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< int8_t > >,
                             singleton_std_list_int8_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< uint16_t > >,
                             singleton_std_list_uint16_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< int16_t > >,
                             singleton_std_list_int16_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< uint32_t > >,
                             singleton_std_list_uint32_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< int32_t > >,
                             singleton_std_list_int32_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< uint64_t > >,
                             singleton_std_list_uint64_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< int64_t > >,
                             singleton_std_list_int64_t );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< float > >,
                             singleton_std_list_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                       std::list< double > >,
                             singleton_std_list_double );