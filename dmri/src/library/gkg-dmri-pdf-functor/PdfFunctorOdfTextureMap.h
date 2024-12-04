#ifndef _gkg_dmri_pdf_functor_PdfFunctorOdfTextureMap_h_
#define _gkg_dmri_pdf_functor_PdfFunctorOdfTextureMap_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorOdfTextureMap :
            public PdfFunctor< TextureMap< OrientationDistributionFunction >,
                               OrientationDistributionFunction >,
            public Singleton< PdfFunctorOdfTextureMap >
{

  public:

    ~PdfFunctorOdfTextureMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( TextureMap< OrientationDistributionFunction >& output,
                  PdfCartesianField& pdfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< PdfFunctorOdfTextureMap >;

    PdfFunctorOdfTextureMap();

};


}


#endif
