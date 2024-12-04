#include <gkg-processing-io/StdVectorBSwapItemReaderOfCurve3d_i.h>


template class gkg::StdVectorBSwapItemReader< gkg::Curve3d< float > >;
template class gkg::StdVectorBSwapItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBSwapItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBSwapItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

