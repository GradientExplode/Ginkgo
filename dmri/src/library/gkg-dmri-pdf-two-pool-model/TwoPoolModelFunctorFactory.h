#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFactory_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class TwoPoolModelFunctorFactory : 
                          public Singleton< TwoPoolModelFunctorFactory< C, I > >
{

  public:

    ~TwoPoolModelFunctorFactory();

    bool registerTwoPoolModelFunctor(
                       const TwoPoolModelFunctor< C, I >& twoPoolModelFunctor );

    const TwoPoolModelFunctor< C, I >&
      getTwoPoolModelFunctor( const std::string& name ) const;

    bool hasTwoPoolModelFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< TwoPoolModelFunctorFactory< C, I > >;

    TwoPoolModelFunctorFactory();

    std::map< std::string,
              const TwoPoolModelFunctor< C, I >* > _twoPoolModelFunctors;

};


}


#endif
