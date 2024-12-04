#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-diffusoid/Diffusoid_i.h>


template class gkg::DiffusoidFunctorFactory<
                                       gkg::Volume< float >,
                                       float >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::Volume< float >,
                                       std::vector< float > >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::Volume< gkg::RGBComponent >,
                                       gkg::RGBComponent >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< std::vector< float > > >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::TextureMap< gkg::Diffusoid >,
                                       gkg::Diffusoid >;
template class gkg::DiffusoidFunctorFactory<
                                       gkg::SiteMap< int32_t, int32_t >,
                                       gkg::Vector3d< int32_t > >;

