#ifndef _gkg_dmri_pdf_functor_PdfMoment_h_
#define _gkg_dmri_pdf_functor_PdfMoment_h_


#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>


namespace gkg
{


class PdfMoment : public DiffusionFeature
{

  public:

    enum BasisType
    {

      Standard,

    };


    PdfMoment( int32_t order, BasisType basisType );
    PdfMoment( const PdfMoment& other );
    virtual ~PdfMoment();

    PdfMoment& operator=( const PdfMoment& other );

    BasisType getBasisType() const;
    std::string getBasisTypeName() const;
    void setBasisType( BasisType basisType );
    void setBasisType( const std::string& basisTypeName );
    void setStandardBasisType();

    int32_t getOrder() const;

  protected:

    int32_t _order;
    BasisType _basisType;

};


}


#endif
