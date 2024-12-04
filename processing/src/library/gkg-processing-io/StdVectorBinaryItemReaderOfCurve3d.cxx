#include <gkg-processing-io/StdVectorBinaryItemReaderOfCurve3d_i.h>


template class gkg::StdVectorBinaryItemReader< gkg::Curve3d< float > >;
template class gkg::StdVectorBinaryItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBinaryItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBinaryItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

