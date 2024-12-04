#include <gkg-processing-io/DefaultAsciiItemReaderOfCurve3dMap_i.h>


template class gkg::DefaultAsciiItemReader< gkg::Curve3dMap< float > >;
template class gkg::DefaultAsciiItemReader< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

