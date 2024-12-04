#include <gkg-processing-io/StdVectorBinaryItemWriterOfLightCurve3d_i.h>


template class gkg::StdVectorBinaryItemWriter< gkg::LightCurve3d< float > >;
template class gkg::StdVectorBinaryItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBinaryItemWriter<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBinaryItemWriter<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

