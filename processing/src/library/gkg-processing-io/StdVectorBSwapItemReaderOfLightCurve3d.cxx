#include <gkg-processing-io/StdVectorBSwapItemReaderOfLightCurve3d_i.h>


template class gkg::StdVectorBSwapItemReader< gkg::LightCurve3d< float > >;
template class gkg::StdVectorBSwapItemReader< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBSwapItemReader<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBSwapItemReader<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

