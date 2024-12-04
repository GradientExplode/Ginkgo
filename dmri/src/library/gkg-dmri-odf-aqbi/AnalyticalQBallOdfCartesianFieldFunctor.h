#ifndef _gkg_dmri_odf_aqbi_AnalyticalQBallOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_aqbi_AnalyticalQBallOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class AnalyticalQBallOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    AnalyticalQBallOdfCartesianFieldFunctor(
     const AnalyticalQBallOdfCartesianField& analyticalQBallOdfCartesianField );
    
    virtual ~AnalyticalQBallOdfCartesianFieldFunctor();

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
