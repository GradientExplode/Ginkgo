#include <gkg-processing-io/DefaultAsciiItemWriterOfCurve3d_i.h>


template class gkg::DefaultAsciiItemWriter< gkg::Curve3d< float > >;
template class gkg::DefaultAsciiItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

