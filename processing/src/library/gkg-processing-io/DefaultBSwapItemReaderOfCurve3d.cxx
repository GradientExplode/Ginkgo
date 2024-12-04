#include <gkg-processing-io/DefaultBSwapItemReaderOfCurve3d_i.h>


template class gkg::DefaultBSwapItemReader< gkg::Curve3d< float > >;
template class gkg::DefaultBSwapItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

