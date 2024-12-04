#include <gkg-processing-io/DefaultBinaryItemWriterOfCurve3d_i.h>


template class gkg::DefaultBinaryItemWriter< gkg::Curve3d< float > >;
template class gkg::DefaultBinaryItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

