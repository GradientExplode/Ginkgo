#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>


template class gkg::EapFunctorFactory< gkg::Volume< float >, float >;
template class gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >;
template class gkg::EapFunctorFactory< gkg::Volume< float >,
                                       std::vector< float > >;
template class gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                       gkg::RGBComponent >;
template class gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >;
template class gkg::EapFunctorFactory< std::vector< 
					 gkg::MeshMap< int32_t, float, 3U > >,
                                       std::vector< float > >;
template class gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< std::vector< float > > >;
template class gkg::EapFunctorFactory< gkg::TextureMap<
				       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >;
template class gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                       gkg::Vector3d< int32_t > >;

