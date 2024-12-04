#include <gkg-processing-io/DefaultAsciiItemWriterOfCurve3dMap_i.h>


template class gkg::DefaultAsciiItemWriter< gkg::Curve3dMap< float > >;
template class gkg::DefaultAsciiItemWriter< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

