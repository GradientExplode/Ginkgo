#include <gkg-processing-io/DefaultBSwapItemReaderOfLightCurve3d_i.h>


template class gkg::DefaultBSwapItemReader< gkg::LightCurve3d< float > >;
template class gkg::DefaultBSwapItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBSwapItemReader<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

