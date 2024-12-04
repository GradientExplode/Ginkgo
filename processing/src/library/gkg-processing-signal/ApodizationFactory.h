#ifndef _gkg_processing_signal_ApodizationFactory_h_
#define _gkg_processing_signal_ApodizationFactory_h_


#include <string>
#include <map>
#include <list>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-signal/Apodization.h>


namespace gkg
{


template < class T >
class ApodizationFactory : public Singleton< ApodizationFactory< T > >
{

  public:

    typedef Apodization< T >* ( *ApodizationCreator )(
                                       int32_t,
                                       typename gkg::Apodization< T >::Type );

    ~ApodizationFactory();

    bool registerFactory( const std::string& name,
                          ApodizationCreator apodizationCreator );

    Apodization< T >*
      createApodization( const std::string& name,
                         int32_t size,
                         typename gkg::Apodization< T >::Type type = 
                                        gkg::Apodization< T >::Standard ) const;

    bool hasApodization( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< ApodizationFactory< T > >;

    ApodizationFactory();

    std::map< std::string, ApodizationCreator > _apodizationCreators;

};


}


#define RegisterApodization( NameSpace, Implementation, Type )             \
static bool init_##NameSpace##_##Implementation##_##Type                   \
         __attribute__((unused)) =                                         \
         gkg::ApodizationFactory< Type >::getInstance().registerFactory(   \
                       NameSpace::Implementation< Type >::getStaticName(), \
                       &NameSpace::Implementation< Type >::createInstance )


#endif
