#ifndef _gkg_dmri_odf_functor_OdfFunctorNematicOrder_h_
#define _gkg_dmri_odf_functor_OdfFunctorNematicOrder_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorNematicOrder : public OdfFunctor< Volume< float >, float >,
                               public Singleton< OdfFunctorNematicOrder >
{

  public:

    ~OdfFunctorNematicOrder();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorNematicOrder >;

    OdfFunctorNematicOrder();

    void processItem(
                 float& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif
