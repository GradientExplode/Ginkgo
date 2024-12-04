#include <gkg-processing-io/DefaultBSwapItemReaderOfCurve3dMap_i.h>


template class gkg::DefaultBSwapItemReader< gkg::Curve3dMap< float > >;
template class gkg::DefaultBSwapItemReader< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

