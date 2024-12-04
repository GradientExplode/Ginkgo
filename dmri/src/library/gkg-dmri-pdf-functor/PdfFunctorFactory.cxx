#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelAttenuation.h>


template class gkg::PdfFunctorFactory< gkg::Volume< float >, float >;
template class gkg::PdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >;
template class gkg::PdfFunctorFactory< gkg::Volume< float >,
                                       std::vector< float > >;
template class gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                       gkg::RGBComponent >;
template class gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >;
template class gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                       std::vector< std::vector< float > > >;
template class gkg::PdfFunctorFactory< gkg::TextureMap<
                                       gkg::ProbabilityDensityFunction >,
                                       gkg::ProbabilityDensityFunction >;
template class gkg::PdfFunctorFactory< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >;
template class gkg::PdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                       gkg::Vector3d< int32_t > >;
template class gkg::PdfFunctorFactory< gkg::MeasurementVsModelPlotter,
                                       gkg::MeasurementVsModelAttenuation >;

