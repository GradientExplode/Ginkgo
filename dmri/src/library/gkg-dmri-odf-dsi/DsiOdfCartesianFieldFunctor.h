#ifndef _gkg_dmri_odf_dsi_DsiOdfCartesianFieldFunctor_h_
#define _gkg_dmri_odf_dsi_DsiOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{

class DsiOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    DsiOdfCartesianFieldFunctor(
				const DsiOdfCartesianField& dsiOdfCartesianField );
      //const Matrix& shMatrixPlus );

    virtual ~DsiOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getReturnToOriginProbability() const;
    const Volume< float >& getMeanSquaredDisplacement() const;
    const Volume< float >& getOdfSphericalHarmonicsCoefficients() const;

  protected:
    
    Volume< float > _returnToOriginProbability;
    Volume< float > _meanSquaredDisplacement;
    Volume< float > _odfSphericalHarmonicsCoefficients;

    Matrix _shMatrixPlus;

};


}


#endif
