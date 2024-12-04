#include <gkg-processing-io/StdVectorBinaryItemWriterOfCurve3d_i.h>


template class gkg::StdVectorBinaryItemWriter< gkg::Curve3d< float > >;
template class gkg::StdVectorBinaryItemWriter< gkg::Curve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBinaryItemWriter<
                                                       gkg::Curve3d< float > >,
                             singleton_curve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBinaryItemWriter<
                                                       gkg::Curve3d< double > >,
                             singleton_curve3d_of_double );

