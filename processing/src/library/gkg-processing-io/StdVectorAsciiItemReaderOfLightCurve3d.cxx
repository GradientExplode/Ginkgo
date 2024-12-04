#include <gkg-processing-io/StdVectorAsciiItemReaderOfLightCurve3d_i.h>


template class gkg::StdVectorAsciiItemReader< gkg::LightCurve3d< float > >;
template class gkg::StdVectorAsciiItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorAsciiItemReader<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorAsciiItemReader<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

