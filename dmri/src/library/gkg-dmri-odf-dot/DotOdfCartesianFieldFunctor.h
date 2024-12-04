#ifndef _gkg_dmri_odf_dot_DotOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_dot_DotOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dot/DotOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class DotOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    DotOdfCartesianFieldFunctor(
                     const DotOdfCartesianField& dotOdfCartesianField );
    
    virtual ~DotOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getDwSphericalHarmonicsCoefficients() const;
    const Volume< float >& getAdcSphericalHarmonicsCoefficients() const;
    const Volume< float >& getOdfSphericalHarmonicsCoefficients() const;

  protected:

    Volume< float > _dwSphericalHarmonicsCoefficients;
    Volume< float > _adcSphericalHarmonicsCoefficients;
    Volume< float > _odfSphericalHarmonicsCoefficients;

};


}


#endif

