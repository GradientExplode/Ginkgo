#include <gkg-processing-io/StdVectorBSwapItemWriterOfCurve3d_i.h>


template class gkg::StdVectorBSwapItemWriter< gkg::Curve3d< float > >;
template class gkg::StdVectorBSwapItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBSwapItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBSwapItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

