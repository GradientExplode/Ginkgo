#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorFactory_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class DiffusoidFunctorFactory :
                            public Singleton< DiffusoidFunctorFactory< C, I > >
{

  public:

    ~DiffusoidFunctorFactory();

    bool registerDiffusoidFunctor(
                            const DiffusoidFunctor< C, I >& diffusoidFunctor );

    const DiffusoidFunctor< C, I >&
      getDiffusoidFunctor( const std::string& name ) const;

    bool hasDiffusoidFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< DiffusoidFunctorFactory< C, I > >;

    DiffusoidFunctorFactory();

    std::map< std::string,
              const DiffusoidFunctor< C, I >* > _diffusoidFunctors;

};


}


#endif
