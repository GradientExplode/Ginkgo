#include <gkg-processing-io/DefaultAsciiItemReaderOfLightCurve3d_i.h>


template class gkg::DefaultAsciiItemReader< gkg::LightCurve3d< float > >;
template class gkg::DefaultAsciiItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                  gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultAsciiItemReader<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

