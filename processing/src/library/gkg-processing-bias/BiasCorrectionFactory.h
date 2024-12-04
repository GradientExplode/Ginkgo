#ifndef _gkg_processing_bias_BiasCorrectionFactory_h_
#define _gkg_processing_bias_BiasCorrectionFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-bias/BiasCorrection.h>
#include <string>
#include <map>
#include <list>
#include <vector>


namespace gkg
{


template < class T >
class BiasCorrectionFactory : public Singleton< BiasCorrectionFactory< T > >
{

  public:

    typedef BiasCorrection< T >* ( *BiasCorrectionCreator )(
                                            const std::vector< double >&,
                                            const std::vector< std::string >& );

    ~BiasCorrectionFactory();

    bool registerFactory( const std::string& name,
                          BiasCorrectionCreator biasCorrectionCreator );

    BiasCorrection< T >*
      createBiasCorrection( const std::string& name,
                            const std::vector< double >&,
                            const std::vector< std::string >& ) const;

    bool hasBiasCorrection( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< BiasCorrectionFactory< T > >;

    BiasCorrectionFactory();

    std::map< std::string, BiasCorrectionCreator > _biasCorrectionCreators;

};


}


#define RegisterBiasCorrection( NameSpace, Implementation, Type )             \
static bool init_##NameSpace##_##Implementation##_##Type                      \
         __attribute((unused)) =                                              \
         gkg::BiasCorrectionFactory< Type >::getInstance().registerFactory(   \
                       NameSpace::Implementation< Type >::getStaticName(),    \
                       &NameSpace::Implementation< Type >::createInstance )


#endif
