#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory_i.h>


template class gkg::ConnectivityMatrixFactory< std::string,
                                               std::less< std::string > >;
template class gkg::ConnectivityMatrixFactory< int16_t,
                                               std::less< int16_t > >;


ForceSingletonInstanciation2( gkg::ConnectivityMatrixFactory<
                                                         int16_t,
                                                         std::less< int16_t > >,
                              gkg_ConnectivityMatrixFactory_int16_t );


ForceSingletonInstanciation2( gkg::ConnectivityMatrixFactory<
                                                     std::string,
                                                     std::less< std::string > >,
                              gkg_ConnectivityMatrixFactory_std_string );


