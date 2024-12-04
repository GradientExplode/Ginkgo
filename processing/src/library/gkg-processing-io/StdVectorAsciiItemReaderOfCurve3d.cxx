#include <gkg-processing-io/StdVectorAsciiItemReaderOfCurve3d_i.h>


template class gkg::StdVectorAsciiItemReader< gkg::Curve3d< float > >;
template class gkg::StdVectorAsciiItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorAsciiItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorAsciiItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

