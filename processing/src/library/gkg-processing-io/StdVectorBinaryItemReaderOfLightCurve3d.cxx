#include <gkg-processing-io/StdVectorBinaryItemReaderOfLightCurve3d_i.h>


template class gkg::StdVectorBinaryItemReader< gkg::LightCurve3d< float > >;
template class gkg::StdVectorBinaryItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBinaryItemReader<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBinaryItemReader<
                                                   gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

