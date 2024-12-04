#ifndef _gkg_processing_resampling_ResamplerFactory_h_
#define _gkg_processing_resampling_ResamplerFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-resampling/Resampler.h>
#include <map>


namespace gkg
{


template < class T >
class ResamplerFactory : public Singleton< ResamplerFactory< T > >
{

  public:

    ~ResamplerFactory();

    void registerResampler( Resampler< T >* resampler );
    Resampler< T >* getResampler( int32_t order ) const;

  protected:

    friend class Singleton< ResamplerFactory< T > >;

    ResamplerFactory();

    std::map< int32_t, Resampler< T >* > _resamplers;

};


}


#define RegisterResampler( NameSpace, Resampler )                            \
static bool registerResampler_##NameSpace##_Resampler()                      \
{                                                                            \
                                                                             \
  gkg::ResamplerFactory< int8_t >::getInstance().registerResampler(          \
                &NameSpace::Resampler< int8_t >::getInstance() );            \
  gkg::ResamplerFactory< uint8_t >::getInstance().registerResampler(         \
                &NameSpace::Resampler< uint8_t >::getInstance() );           \
  gkg::ResamplerFactory< int16_t >::getInstance().registerResampler(         \
                &NameSpace::Resampler< int16_t >::getInstance() );           \
  gkg::ResamplerFactory< uint16_t >::getInstance().registerResampler(        \
                &NameSpace::Resampler< uint16_t >::getInstance() );          \
  gkg::ResamplerFactory< int32_t >::getInstance().registerResampler(         \
                &NameSpace::Resampler< int32_t >::getInstance() );           \
  gkg::ResamplerFactory< uint32_t >::getInstance().registerResampler(        \
                &NameSpace::Resampler< uint32_t >::getInstance() );          \
  gkg::ResamplerFactory< int64_t >::getInstance().registerResampler(         \
                &NameSpace::Resampler< int64_t >::getInstance() );           \
  gkg::ResamplerFactory< uint64_t >::getInstance().registerResampler(        \
                &NameSpace::Resampler< uint64_t >::getInstance() );          \
  gkg::ResamplerFactory< float >::getInstance().registerResampler(           \
                &NameSpace::Resampler< float >::getInstance() );             \
  gkg::ResamplerFactory< double >::getInstance().registerResampler(          \
                &NameSpace::Resampler< double >::getInstance() );            \
  gkg::ResamplerFactory< std::complex< float > >::getInstance().             \
                                                  registerResampler(         \
           &NameSpace::Resampler< std::complex< float > >::getInstance() );  \
  gkg::ResamplerFactory< std::complex< double > >::getInstance().            \
                                                  registerResampler(         \
           &NameSpace::Resampler< std::complex< double > >::getInstance() ); \
  gkg::ResamplerFactory< gkg::RGBComponent >::getInstance().                 \
                                                  registerResampler(         \
           &NameSpace::Resampler< gkg::RGBComponent >::getInstance() );      \
  return true;                                                               \
                                                                             \
}                                                                            \
                                                                             \
static bool registeredResampler_##NameSpace##_Resampler                      \
  __attribute__((unused)) =                                                  \
  registerResampler_##NameSpace##_Resampler()

#endif
