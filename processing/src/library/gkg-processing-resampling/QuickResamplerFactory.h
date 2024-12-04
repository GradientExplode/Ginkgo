#ifndef _gkg_processing_resampling_QuickResamplerFactory_h_
#define _gkg_processing_resampling_QuickResamplerFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <map>


namespace gkg
{


template < class T >
class QuickResamplerFactory : public Singleton< QuickResamplerFactory< T > >
{

  public:

    ~QuickResamplerFactory();

    void registerQuickResampler( QuickResampler< T >* spreader );
    QuickResampler< T >* getQuickResampler( const std::string& name ) const;

  protected:

    friend class Singleton< QuickResamplerFactory< T > >;

    QuickResamplerFactory();

    std::map< std::string, QuickResampler< T >* > _quickResamplers;

};


}


#define RegisterQuickResampler( NameSpace, QuickResampler )                    \
static bool registerQuickResampler_##NameSpace##_QuickResampler()              \
{                                                                              \
                                                                               \
  gkg::QuickResamplerFactory< int8_t >::getInstance().registerQuickResampler(  \
                &NameSpace::QuickResampler< int8_t >::getInstance() );         \
  gkg::QuickResamplerFactory< uint8_t >::getInstance().                        \
                                                     registerQuickResampler(   \
                &NameSpace::QuickResampler< uint8_t >::getInstance() );        \
  gkg::QuickResamplerFactory< int16_t >::getInstance().registerQuickResampler( \
                &NameSpace::QuickResampler< int16_t >::getInstance() );        \
  gkg::QuickResamplerFactory< uint16_t >::getInstance().                       \
                                                      registerQuickResampler(  \
                &NameSpace::QuickResampler< uint16_t >::getInstance() );       \
  gkg::QuickResamplerFactory< int32_t >::getInstance().registerQuickResampler( \
                &NameSpace::QuickResampler< int32_t >::getInstance() );        \
  gkg::QuickResamplerFactory< uint32_t >::getInstance().                       \
                                                    registerQuickResampler(    \
                &NameSpace::QuickResampler< uint32_t >::getInstance() );       \
  gkg::QuickResamplerFactory< int64_t >::getInstance().registerQuickResampler( \
                &NameSpace::QuickResampler< int64_t >::getInstance() );        \
  gkg::QuickResamplerFactory< uint64_t >::getInstance().                       \
                                                     registerQuickResampler(   \
                &NameSpace::QuickResampler< uint64_t >::getInstance() );       \
  gkg::QuickResamplerFactory< float >::getInstance().registerQuickResampler(   \
                &NameSpace::QuickResampler< float >::getInstance() );          \
  gkg::QuickResamplerFactory< double >::getInstance().registerQuickResampler(  \
                &NameSpace::QuickResampler< double >::getInstance() );         \
  gkg::QuickResamplerFactory< std::complex< float > >::getInstance().          \
                                                 registerQuickResampler(       \
          &NameSpace::QuickResampler< std::complex< float > >::getInstance() );\
  gkg::QuickResamplerFactory< std::complex< double > >::getInstance().         \
                                                 registerQuickResampler(       \
         &NameSpace::QuickResampler< std::complex< double > >::getInstance() );\
  gkg::QuickResamplerFactory< gkg::RGBComponent >::getInstance().              \
                                                 registerQuickResampler(       \
         &NameSpace::QuickResampler< gkg::RGBComponent >::getInstance() );     \
  return true;                                                                 \
                                                                               \
}                                                                              \
                                                                               \
static bool registeredQuickResampler_##NameSpace##_QuickResampler              \
  __attribute__((unused)) =                                                    \
  registerQuickResampler_##NameSpace##_QuickResampler()

#endif
