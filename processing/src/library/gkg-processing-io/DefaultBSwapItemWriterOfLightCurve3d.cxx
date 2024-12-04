#include <gkg-processing-io/DefaultBSwapItemWriterOfLightCurve3d_i.h>


template class gkg::DefaultBSwapItemWriter< gkg::LightCurve3d< float > >;
template class gkg::DefaultBSwapItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemWriter<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

