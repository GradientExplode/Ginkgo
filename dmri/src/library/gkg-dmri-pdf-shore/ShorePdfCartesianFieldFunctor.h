#ifndef _gkg_dmri_pdf_shore_ShorePdfCartesianFieldFunctor_h_
#define _gkg_dmri_pdf_shore_ShorePdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-pdf-shore/ShorePdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class ShorePdfCartesianFieldFunctor :
             public DiffusionCartesianFieldFunctor< ProbabilityDensityFunction >
{

  public:


    ShorePdfCartesianFieldFunctor(
                         const ShorePdfCartesianField& shorePdfCartesianField );
    virtual ~ShorePdfCartesianFieldFunctor();

    ProbabilityDensityFunction&
      apply( const Vector3d< int32_t >& site,
             ProbabilityDensityFunction& pdf );

    const Volume< float >& getPdfPolarHarmonicsCoefficients() const;

  protected:

    Vector3d< int32_t > _size;
    Vector3d< double > _resolution;

    Volume< float > _pdfPolarHarmonicsCoefficients;

};


}


#endif
