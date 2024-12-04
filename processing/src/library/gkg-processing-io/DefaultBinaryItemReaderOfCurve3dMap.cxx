#include <gkg-processing-io/DefaultBinaryItemReaderOfCurve3dMap_i.h>


template class gkg::DefaultBinaryItemReader< gkg::Curve3dMap< float > >;
template class gkg::DefaultBinaryItemReader< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

