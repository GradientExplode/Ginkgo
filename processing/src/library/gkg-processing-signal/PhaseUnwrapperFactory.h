#ifndef _gkg_processing_signal_PhaseUnwrapperFactory_h_
#define _gkg_processing_signal_PhaseUnwrapperFactory_h_


#include <string>
#include <map>
#include <list>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-signal/PhaseUnwrapper.h>


namespace gkg
{


template < class T >
class PhaseUnwrapperFactory : public Singleton< PhaseUnwrapperFactory< T > >
{

  public:

    typedef PhaseUnwrapper< T >* ( *PhaseUnwrapperCreator )( void );

    ~PhaseUnwrapperFactory();

    bool registerFactory( const std::string& name,
                          PhaseUnwrapperCreator phaseUnwrapperCreator );

    PhaseUnwrapper< T >*
      createPhaseUnwrapper( const std::string& name ) const;

    bool hasPhaseUnwrapper( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< PhaseUnwrapperFactory< T > >;

    PhaseUnwrapperFactory();

    std::map< std::string, PhaseUnwrapperCreator > _phaseUnwrapperCreators;

};


}


#define RegisterPhaseUnwrapper( NameSpace, Implementation, Type )           \
static bool init_##NameSpace##_##Implementation##_##Type                    \
         __attribute__((unused)) =                                          \
         gkg::PhaseUnwrapperFactory< Type >::getInstance().registerFactory( \
                       NameSpace::Implementation< Type >::getStaticName(),  \
                       &NameSpace::Implementation< Type >::createInstance )


#endif
