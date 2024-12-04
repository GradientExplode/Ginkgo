#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-pdf-dti/Tensor.h>


template class gkg::TensorFunctorFactory<
                                         gkg::Volume< float >,
                                         float >;
template class gkg::TensorFunctorFactory<
                                         gkg::Volume< gkg::Vector3d< float > >,
                                         gkg::Vector3d< float > >;
template class gkg::TensorFunctorFactory<
                                         gkg::Volume< float >,
                                         std::vector< float > >;
template class gkg::TensorFunctorFactory<
                                         gkg::Volume< gkg::RGBComponent >,
                                         gkg::RGBComponent >;
template class gkg::TensorFunctorFactory<
                                         gkg::TextureMap< gkg::Tensor >,
                                         gkg::Tensor >;
template class gkg::TensorFunctorFactory<
                                         gkg::SiteMap< int32_t, int32_t >,
                                         gkg::Vector3d< int32_t > >;

