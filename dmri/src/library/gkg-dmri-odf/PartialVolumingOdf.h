#ifndef _gkg_dmri_odf_PartialVolumingOdf_h_
#define _gkg_dmri_odf_PartialVolumingOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-container/OdfCartesianField.h>


namespace gkg
{


class OdfCartesianField;


class PartialVolumingOdf : public OrientationDistributionFunction
{

  public:

    PartialVolumingOdf( const OdfCartesianField& odfCartesianField,
                        const Vector3d< float >& site,
                        const Vector3d< int32_t >& sizes,
                        const Vector3d< double >& resolution );
    PartialVolumingOdf( const PartialVolumingOdf& other );
    virtual ~PartialVolumingOdf();

  protected:

    void partialVolumingCoordinateAndWeight( const int32_t& size,
                                             const float& realCoordinate,
                                             const double& resolution,
                                             int32_t& inferiorIntegerCoordinate,
                                             int32_t& superiorIntegerCoordinate,
                                             float& weight ) const;

};


}


#endif
