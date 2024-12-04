#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>


template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::Volume< float >,
                                         float >;
template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::Volume< gkg::Vector3d< float > >,
                                         gkg::Vector3d< float > >;
template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::Volume< float >,
                                         std::vector< float > >;
template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::Volume< gkg::RGBComponent >,
                                         gkg::RGBComponent >;
template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::TextureMap< gkg::TwoPoolModel >,
                                         gkg::TwoPoolModel >;
template class gkg::TwoPoolModelFunctorFactory<
                                         gkg::SiteMap< int32_t, int32_t >,
                                         gkg::Vector3d< int32_t > >;

