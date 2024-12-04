#ifndef _gkg_processing_io_TextureMapAnalyzer_h_
#define _gkg_processing_io_TextureMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class TextureMapAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< TextureMapAnalyzer< T > >
{

  public:

    ~TextureMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< TextureMapAnalyzer< T > >;

    TextureMapAnalyzer();

};


}


//
// forcing instanciation of some default TextureMap analyzers is useless
// since there can be a large set of types for template parameter T;
// so, here is an example of singleton instanciation and analyzer registration
// that can be used as a model; here, we desinged a texture map of QBall items
//
//
// #include <gkg-processing-io/TextureMapAnalyzer_i.h>
// #include <gkg-core-exception/Exception.h>
//
// namespace gkg
// {
//
// template <>
// gkg::TextureMapAnalyzer< gkg::QBallModel >*
//    gkg::Singleton< gkg::TextureMapAnalyzer< gkg::QBallModel >
//                  >::_instance = 0;
//
// }
//
// static bool initialize()
// {
//
//   try
//   {
//
//     gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
//       gkg::TextureMapDiskFormat< gkg::QBallModel >::getInstance().getName(),
//       &gkg::TextureMapAnalyzer< gkg::QBallModel >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "Texture Map analyzer registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterTextureAnalyzer( TYPE, NAME )                     \
static bool initializeAnalyzer##NAME()                            \
{                                                                 \
                                                                  \
  try                                                             \
  {                                                               \
                                                                  \
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(      \
      gkg::TextureMapDiskFormat< TYPE                             \
                               >::getInstance().getName(),        \
      &gkg::TextureMapAnalyzer<TYPE >::getInstance() );           \
                                                                  \
    return true;                                                  \
                                                                  \
  }                                                               \
  GKG_CATCH( "Texture Map analyzer registration: " )              \
                                                                  \
}                                                                 \
                                                                  \
static bool initializedAnalyzer##NAME __attribute__((unused)) =   \
  initializeAnalyzer##NAME()


#endif

