#ifndef _gkg_dmri_pdf_functor_PdfFunctorRGB_h_
#define _gkg_dmri_pdf_functor_PdfFunctorRGB_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorRGB : public PdfFunctor< Volume< RGBComponent >, RGBComponent >,
                      public Singleton< PdfFunctorRGB >
{

  public:

    ~PdfFunctorRGB();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< RGBComponent >& output,
                  PdfCartesianField& pdfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< PdfFunctorRGB >;

    PdfFunctorRGB();

};


}


#endif
