#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelAttenuation.h>


template class gkg::OdfFunctorFactory< gkg::Volume< float >, float >;
template class gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >;
template class gkg::OdfFunctorFactory< gkg::Volume< float >,
                                       std::vector< float > >;
template class gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                       gkg::RGBComponent >;
template class gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >;
template class gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< std::vector< float > > >;
template class gkg::OdfFunctorFactory< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >;
template class gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                       gkg::Vector3d< int32_t > >;
template class gkg::OdfFunctorFactory< gkg::MeasurementVsModelPlotter,
                                       gkg::MeasurementVsModelAttenuation >;
template class gkg::OdfFunctorFactory< 
                       gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
                       std::vector< gkg::Vector3d< float > > >;

