#include <gkg-processing-io/StdVectorBSwapItemWriterOfLightCurve3d_i.h>


template class gkg::StdVectorBSwapItemWriter< gkg::LightCurve3d< float > >;
template class gkg::StdVectorBSwapItemWriter< gkg::LightCurve3d< double > >;

ForceSingletonInstanciation( gkg::StdVectorBSwapItemWriter<
                                                   gkg::LightCurve3d< float > >,
                             singleton_lightcurve3d_of_float );
ForceSingletonInstanciation( gkg::StdVectorBSwapItemWriter<
                                                  gkg::LightCurve3d< double > >,
                             singleton_lightcurve3d_of_double );

