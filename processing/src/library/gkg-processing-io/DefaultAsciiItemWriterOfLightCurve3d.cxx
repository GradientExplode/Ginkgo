#include <gkg-processing-io/DefaultAsciiItemWriterOfLightCurve3d_i.h>


template class gkg::DefaultAsciiItemWriter< gkg::LightCurve3d< float > >;
template class gkg::DefaultAsciiItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemWriter<
                                                gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

