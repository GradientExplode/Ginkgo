#include <gkg-processing-io/StdVectorAsciiItemWriterOfCurve3d_i.h>


template class gkg::StdVectorAsciiItemWriter< gkg::Curve3d< float > >;
template class gkg::StdVectorAsciiItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorAsciiItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorAsciiItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

