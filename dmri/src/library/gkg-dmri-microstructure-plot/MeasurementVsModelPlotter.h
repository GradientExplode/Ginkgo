#ifndef _gkg_dmri_microstructure_plot_MeasurementVsModelPlotter_h_
#define _gkg_dmri_microstructure_plot_MeasurementVsModelPlotter_h_


#include <gkg-processing-container/CartesianField.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelAttenuation.h>
#include <string>


namespace gkg
{


class MeasurementVsModelPlotter :
                         public CartesianField< MeasurementVsModelAttenuation >
{

  public:

    MeasurementVsModelPlotter( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const Vector3d< double >& resolution,
                               const SiteMap< int32_t, int32_t >& siteMap,
                               int32_t offsetX = 0,
                               int32_t offsetY = 0,
                               int32_t offsetZ = 0 );
    MeasurementVsModelPlotter(
                       int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                       const Vector3d< double >& resolution,
                       const SiteMap< int32_t, int32_t >& siteMap,
                       const TextureMap< MeasurementVsModelAttenuation >& items,
                       int32_t offsetX = 0,
                       int32_t offsetY = 0,
                       int32_t offsetZ = 0 );
    MeasurementVsModelPlotter(
                     const SiteMap< int32_t, int32_t >& siteMap,
                     const TextureMap< MeasurementVsModelAttenuation >& items );
    MeasurementVsModelPlotter( int32_t offsetX = 0,
                               int32_t offsetY = 0,
                               int32_t offsetZ = 0 );
    virtual ~MeasurementVsModelPlotter();

    void write( const std::string& outputDirectory ) const;

};


}



#endif
