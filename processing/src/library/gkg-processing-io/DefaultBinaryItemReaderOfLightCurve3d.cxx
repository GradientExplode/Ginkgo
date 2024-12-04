#include <gkg-processing-io/DefaultBinaryItemReaderOfLightCurve3d_i.h>


template class gkg::DefaultBinaryItemReader< gkg::LightCurve3d< float > >;
template class gkg::DefaultBinaryItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::DefaultBinaryItemReader<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

