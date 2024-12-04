#ifndef _gkg_dmri_pdf_functor_EapFunctorFactory_h_
#define _gkg_dmri_pdf_functor_EapFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class EapFunctorFactory : public Singleton< EapFunctorFactory< C, I > >
{

  public:

    ~EapFunctorFactory();

    bool registerEapFunctor( const EapFunctor< C, I >& eapFunctor );

    const EapFunctor< C, I >& getEapFunctor( const std::string& name ) const;
    
    bool hasEapFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< EapFunctorFactory< C, I > >;

    EapFunctorFactory();

    std::map< std::string, const EapFunctor< C, I >* > _eapFunctors;

};


}


#endif
