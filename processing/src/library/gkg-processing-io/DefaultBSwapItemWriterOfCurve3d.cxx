#include <gkg-processing-io/DefaultBSwapItemWriterOfCurve3d_i.h>


template class gkg::DefaultBSwapItemWriter< gkg::Curve3d< float > >;
template class gkg::DefaultBSwapItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

