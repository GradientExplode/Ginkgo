#ifndef _gkg_dmri_pdf_functor_EapFunctorGFA_h_
#define _gkg_dmri_pdf_functor_EapFunctorGFA_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class EapFunctorGFA : public EapFunctor< Volume< float >, float >,
                      public Singleton< EapFunctorGFA >
{

  public:

    ~EapFunctorGFA();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< EapFunctorGFA >;

    EapFunctorGFA();

    void processItem(
                 float& item,
                 const OrientationDistributionFunction& eap,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif
