#ifndef _gkg_dmri_pdf_dpi_DpiEapCartesianFieldFunctor_h_
#define _gkg_dmri_pdf_dpi_DpiEapCartesianFieldFunctor_h_

#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class DpiEapCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    DpiEapCartesianFieldFunctor(
                     const DpiEapCartesianField& dpiEapCartesianField );
    virtual ~DpiEapCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& eap );

    const Volume< float >& getDwSphericalHarmonicsCoefficients() const;
    const Volume< float >& getEapSphericalHarmonicsCoefficients() const;
    const Volume< float >& getAverageNormalizedDwSignal() const;
    const Volume< float >& getAnalyticalEapAtZero() const;
    const Volume< float >& getMeanSquaredDisplacement() const;

  protected:

    Volume< float > _dwSphericalHarmonicsCoefficients;
    Volume< float > _eapSphericalHarmonicsCoefficients;
    Volume< float > _averageNormalizedDwSignal;
    Volume< float > _analyticalEapAtZero;
    Volume< float > _meanSquaredDisplacement;

};


}


#endif
