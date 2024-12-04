#ifndef _gkg_processing_resampling_SpreaderFactory_h_
#define _gkg_processing_resampling_SpreaderFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-resampling/Spreader.h>
#include <map>


namespace gkg
{


template < class T >
class SpreaderFactory : public Singleton< SpreaderFactory< T > >
{

  public:

    ~SpreaderFactory();

    void registerSpreader( Spreader< T >* spreader );
    Spreader< T >* getSpreader( const std::string& name ) const;

  protected:

    friend class Singleton< SpreaderFactory< T > >;

    SpreaderFactory();

    std::map< std::string, Spreader< T >* > _spreaders;

};


}


#define RegisterSpreader( NameSpace, Spreader )                            \
static bool registerSpreader_##NameSpace##_Spreader()                      \
{                                                                          \
                                                                           \
  gkg::SpreaderFactory< int8_t >::getInstance().registerSpreader(          \
                &NameSpace::Spreader< int8_t >::getInstance() );           \
  gkg::SpreaderFactory< uint8_t >::getInstance().registerSpreader(         \
                &NameSpace::Spreader< uint8_t >::getInstance() );          \
  gkg::SpreaderFactory< int16_t >::getInstance().registerSpreader(         \
                &NameSpace::Spreader< int16_t >::getInstance() );          \
  gkg::SpreaderFactory< uint16_t >::getInstance().registerSpreader(        \
                &NameSpace::Spreader< uint16_t >::getInstance() );         \
  gkg::SpreaderFactory< int32_t >::getInstance().registerSpreader(         \
                &NameSpace::Spreader< int32_t >::getInstance() );          \
  gkg::SpreaderFactory< uint32_t >::getInstance().registerSpreader(        \
                &NameSpace::Spreader< uint32_t >::getInstance() );         \
  gkg::SpreaderFactory< int64_t >::getInstance().registerSpreader(         \
                &NameSpace::Spreader< int64_t >::getInstance() );          \
  gkg::SpreaderFactory< uint64_t >::getInstance().registerSpreader(        \
                &NameSpace::Spreader< uint64_t >::getInstance() );         \
  gkg::SpreaderFactory< float >::getInstance().registerSpreader(           \
                &NameSpace::Spreader< float >::getInstance() );            \
  gkg::SpreaderFactory< double >::getInstance().registerSpreader(          \
                &NameSpace::Spreader< double >::getInstance() );           \
  gkg::SpreaderFactory< std::complex< float > >::getInstance().            \
                                                 registerSpreader(         \
          &NameSpace::Spreader< std::complex< float > >::getInstance() );  \
  gkg::SpreaderFactory< std::complex< double > >::getInstance().           \
                                                 registerSpreader(         \
          &NameSpace::Spreader< std::complex< double > >::getInstance() ); \
  gkg::SpreaderFactory< gkg::RGBComponent >::getInstance().                \
                                                 registerSpreader(         \
          &NameSpace::Spreader< gkg::RGBComponent >::getInstance() );      \
  return true;                                                             \
                                                                           \
}                                                                          \
                                                                           \
static bool registeredSpreader_##NameSpace##_Spreader                      \
  __attribute__((unused)) =                                                \
  registerSpreader_##NameSpace##_Spreader()

#endif
