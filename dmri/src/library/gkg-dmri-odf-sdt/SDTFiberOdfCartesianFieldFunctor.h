#ifndef _gkg_dmri_odf_sdt_SDTFiberOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_sdt_SDTFiberOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class SDTFiberOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    SDTFiberOdfCartesianFieldFunctor(
                  const SDTFiberOdfCartesianField& sdtFiberOdfCartesianField );
    virtual ~SDTFiberOdfCartesianFieldFunctor();

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

