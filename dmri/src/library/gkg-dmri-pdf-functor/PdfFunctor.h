#ifndef _gkg_dmri_pdf_functor_PdfFunctor_h_
#define _gkg_dmri_pdf_functor_PdfFunctor_h_


#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class PdfFunctor
{

  public:

    virtual ~PdfFunctor();

    virtual std::string getName() const = 0;
    virtual bool isStandardBasisRequired() const = 0;

    virtual void process( C& output,
                          PdfCartesianField& pdfCartesianField,
                          const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters,
                          bool verbose ) const;

  protected:

    PdfFunctor();

    // RGB scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]
    // lower GFA threshold must be stored in scalarParameters[ 2 ]
    // upper GFA threshold must be stored in scalarParameters[ 3 ]

    virtual void processItem(
                I& item,
                const ProbabilityDensityFunction& pdf,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
