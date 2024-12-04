#ifndef _gkg_dmri_pdf_dti_TensorFunctorFactory_h_
#define _gkg_dmri_pdf_dti_TensorFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class TensorFunctorFactory : public Singleton< TensorFunctorFactory< C, I > >
{

  public:

    ~TensorFunctorFactory();

    bool registerTensorFunctor( const TensorFunctor< C, I >& tensorFunctor );

    const TensorFunctor< C, I >& getTensorFunctor( const std::string& name ) const;

    bool hasTensorFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< TensorFunctorFactory< C, I > >;

    TensorFunctorFactory();

    std::map< std::string, const TensorFunctor< C, I >* > _tensorFunctors;

};


}


#endif
