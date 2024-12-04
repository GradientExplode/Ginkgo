#ifndef _gkg_dmri_microstructure_plot_MeasurementVsModelAttenuation_h_
#define _gkg_dmri_microstructure_plot_MeasurementVsModelAttenuation_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>


namespace gkg
{


class MeasurementVsModelAttenuation
{

  public:

    MeasurementVsModelAttenuation();
    MeasurementVsModelAttenuation(
          const std::vector< float >& measuredAttenuationValues,
          const OrientationSet& inputOrientationSet,
          const Vector3d< float >& principalOrientation,

          const std::vector< std::vector< float > >& modelededAttenuationValues,
          const std::vector< float >& multipleShellBValues,
          const std::vector< std::vector< int32_t > >&
                                              multipleShellOrientationIndexSets,
          const std::vector< float >& coordinates );
    MeasurementVsModelAttenuation(
                                  const MeasurementVsModelAttenuation& other );
    virtual ~MeasurementVsModelAttenuation();

    MeasurementVsModelAttenuation& operator=( 
                                  const MeasurementVsModelAttenuation& other );

    void write( const std::string& fileName ) const;

  protected:

    std::vector< std::string > _labels;

    std::vector< std::vector< float > > _measuredXs;
    std::vector< std::vector< float > > _measuredYs;

    std::vector< std::vector< float > > _modeledXs;
    std::vector< std::vector< float > > _modeledYs;


};


}




#endif

