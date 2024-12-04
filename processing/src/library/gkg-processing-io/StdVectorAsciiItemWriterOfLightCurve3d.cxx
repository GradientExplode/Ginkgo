#include <gkg-processing-io/StdVectorAsciiItemWriterOfLightCurve3d_i.h>


template class gkg::StdVectorAsciiItemWriter< gkg::LightCurve3d< float > >;
template class gkg::StdVectorAsciiItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorAsciiItemWriter<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorAsciiItemWriter<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );
