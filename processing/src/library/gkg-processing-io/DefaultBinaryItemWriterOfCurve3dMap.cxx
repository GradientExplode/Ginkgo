#include <gkg-processing-io/DefaultBinaryItemWriterOfCurve3dMap_i.h>


template class gkg::DefaultBinaryItemWriter< gkg::Curve3dMap< float > >;
template class gkg::DefaultBinaryItemWriter< gkg::Curve3dMap< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                    gkg::Curve3dMap< float > >,
                             singleton_curve3dmap_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                    gkg::Curve3dMap< double > >,
                             singleton_curve3dmap_of_double );

