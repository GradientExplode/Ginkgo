#ifndef _gkg_dmri_odf_functor_OdfFunctorKappa_h_
#define _gkg_dmri_odf_functor_OdfFunctorKappa_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorKappa : public OdfFunctor< Volume< float >, float >,
                        public Singleton< OdfFunctorKappa >
{

  public:

    ~OdfFunctorKappa();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& /*scalarParameters*/,
                  const std::vector< std::string >& /*stringParameters*/,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorKappa >;

    OdfFunctorKappa();

};


}


#endif
