#include <gkg-processing-io/DefaultBinaryItemWriterOfLightCurve3d_i.h>


template class gkg::DefaultBinaryItemWriter< gkg::LightCurve3d< float > >;
template class gkg::DefaultBinaryItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemWriter<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

