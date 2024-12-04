#ifndef _gkg_dmri_odf_msmt_csd_MSMTCSDFiberOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_msmt_csd_MSMTCSDFiberOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-msmt-csd/MSMTCSDFiberOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class MSMTCSDFiberOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    MSMTCSDFiberOdfCartesianFieldFunctor(
           const MSMTCSDFiberOdfCartesianField& msmtCSDFiberOdfCartesianField );
    virtual ~MSMTCSDFiberOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getOdfAllSphericalHarmonicsCoefficients() const;

  protected:

    Volume< float > _odfAllSphericalHarmonicsCoefficients;

};


}


#endif

