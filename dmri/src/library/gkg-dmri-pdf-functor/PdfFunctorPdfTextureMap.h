#ifndef _gkg_dmri_pdf_functor_PdfFunctorPdfTextureMap_h_
#define _gkg_dmri_pdf_functor_PdfFunctorPdfTextureMap_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorPdfTextureMap :
            public PdfFunctor< TextureMap< ProbabilityDensityFunction >,
                               ProbabilityDensityFunction >,
            public Singleton< PdfFunctorPdfTextureMap >
{

  public:

    ~PdfFunctorPdfTextureMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( TextureMap< ProbabilityDensityFunction >& output,
                  PdfCartesianField& pdfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< PdfFunctorPdfTextureMap >;

    PdfFunctorPdfTextureMap();

};


}


#endif
