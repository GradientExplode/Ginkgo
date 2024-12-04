#ifndef _gkg_dmri_odf_functor_OdfFunctorFactory_h_
#define _gkg_dmri_odf_functor_OdfFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class OdfFunctorFactory : public Singleton< OdfFunctorFactory< C, I > >
{

  public:

    ~OdfFunctorFactory();

    bool registerOdfFunctor( const OdfFunctor< C, I >& odfFunctor );

    const OdfFunctor< C, I >& getOdfFunctor( const std::string& name ) const;

    bool hasOdfFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< OdfFunctorFactory< C, I > >;

    OdfFunctorFactory();

    std::map< std::string, const OdfFunctor< C, I >* > _odfFunctors;

};


}


#endif
