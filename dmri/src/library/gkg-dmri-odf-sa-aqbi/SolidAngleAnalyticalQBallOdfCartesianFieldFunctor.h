#ifndef _gkg_dmri_odf_sa_aqbi_SolidAngleAnalyticalQBallOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_sa_aqbi_SolidAngleAnalyticalQBallOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class SolidAngleAnalyticalQBallOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    SolidAngleAnalyticalQBallOdfCartesianFieldFunctor(
     const SolidAngleAnalyticalQBallOdfCartesianField&
                                    soldAngleAnalyticalQBallOdfCartesianField );
    
    virtual ~SolidAngleAnalyticalQBallOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getDwSphericalHarmonicsCoefficients() const;
    const Volume< float >& getOdfSphericalHarmonicsCoefficients() const;

  protected:

    Volume< float > _dwSphericalHarmonicsCoefficients;
    Volume< float > _odfSphericalHarmonicsCoefficients;

};


}


#endif
