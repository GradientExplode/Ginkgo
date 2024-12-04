#include <gkg-processing-io/DefaultBinaryItemReaderOfCurve3d_i.h>


template class gkg::DefaultBinaryItemReader< gkg::Curve3d< float > >;
template class gkg::DefaultBinaryItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

