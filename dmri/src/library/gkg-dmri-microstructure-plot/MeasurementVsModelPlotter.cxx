#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>


gkg::MeasurementVsModelPlotter::MeasurementVsModelPlotter(
                                int32_t sizeX,
                                int32_t sizeY,
                                int32_t sizeZ,
                                const gkg::Vector3d< double >& resolution,
                                const gkg::SiteMap< int32_t, int32_t >& siteMap,
                                int32_t offsetX,
                                int32_t offsetY,
                                int32_t offsetZ )
                               : gkg::CartesianField<
                                           gkg::MeasurementVsModelAttenuation >(
                                                              sizeX, 
							      sizeY,
                                                              sizeZ,
                                                              resolution,
                                                              siteMap,
                                                              offsetX,
                                                              offsetY,
                                                              offsetZ )
{
}


gkg::MeasurementVsModelPlotter::MeasurementVsModelPlotter(
             int32_t sizeX,
             int32_t sizeY,
             int32_t sizeZ,
             const gkg::Vector3d< double >& resolution,
             const gkg::SiteMap< int32_t, int32_t >& siteMap,
             const gkg::TextureMap< gkg::MeasurementVsModelAttenuation >& items,
             int32_t offsetX,
             int32_t offsetY,
             int32_t offsetZ )
                               : gkg::CartesianField<
                                           gkg::MeasurementVsModelAttenuation >(
                                                              sizeX, 
							      sizeY,
                                                              sizeZ,
                                                              resolution,
                                                              siteMap,
                                                              items,
                                                              offsetX,
                                                              offsetY,
                                                              offsetZ )
{
}


gkg::MeasurementVsModelPlotter::MeasurementVsModelPlotter(
            const gkg::SiteMap< int32_t, int32_t >& siteMap,
            const gkg::TextureMap< gkg::MeasurementVsModelAttenuation >& items )
                               : gkg::CartesianField<
                                           gkg::MeasurementVsModelAttenuation >(
                                                              siteMap,
                                                              items )
{
}


gkg::MeasurementVsModelPlotter::MeasurementVsModelPlotter( int32_t offsetX,
                                                           int32_t offsetY,
                                                           int32_t offsetZ )
                               : gkg::CartesianField<
                                           gkg::MeasurementVsModelAttenuation >(
                                                              offsetX,
                                                              offsetY,
                                                              offsetZ )
{
}


gkg::MeasurementVsModelPlotter::~MeasurementVsModelPlotter()
{
}


void gkg::MeasurementVsModelPlotter::write(
                                      const std::string& outputDirectory ) const
{

  try
  {

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t sizeX = _lut.getSizeX();
    int32_t sizeY = _lut.getSizeY();
    int32_t sizeZ = _lut.getSizeZ();

    gkg::Directory directory( outputDirectory );

    if ( !directory.isValid() )
    {

      directory.mkdir();

    }

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          gkg::MeasurementVsModelAttenuation* 
            measurementVsModelAttenuation = _lut( x, y, z );

          if ( measurementVsModelAttenuation )
          {

            std::string fileName = outputDirectory + 
                                   gkg::getDirectorySeparator() + 
                                   gkg::StringConverter::toString( x ) + "_" +
                                   gkg::StringConverter::toString( y ) + "_" +
                                   gkg::StringConverter::toString( z ) + ".py";
            measurementVsModelAttenuation->write( fileName );

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::MeasurementVsModelPlotter::write( "
             "const std::string& outputDirectory ) const" );

}


