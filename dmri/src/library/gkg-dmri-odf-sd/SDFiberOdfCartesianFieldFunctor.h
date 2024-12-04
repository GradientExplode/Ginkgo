#ifndef _gkg_dmri_odf_sd_SHFiberOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_sd_SHFiberOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sd/SDFiberOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class SDFiberOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    SDFiberOdfCartesianFieldFunctor(
                    const SDFiberOdfCartesianField& sdFiberOdfCartesianField );
    virtual ~SDFiberOdfCartesianFieldFunctor();

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

