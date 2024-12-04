#include <gkg-processing-io/DefaultBSwapItemWriterOfCurve3dMap_i.h>


template class gkg::DefaultBSwapItemWriter< gkg::Curve3dMap< float > >;
template class gkg::DefaultBSwapItemWriter< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

