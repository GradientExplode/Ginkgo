#include <gkg-processing-io/DefaultAsciiItemReaderOfCurve3d_i.h>


template class gkg::DefaultAsciiItemReader< gkg::Curve3d< float > >;
template class gkg::DefaultAsciiItemReader< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

