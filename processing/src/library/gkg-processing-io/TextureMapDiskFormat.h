#ifndef _gkg_processing_io_TextureMapDiskFormat_h_
#define _gkg_processing_io_TextureMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class TextureMap;


template < class T  >
class TextureMapDiskFormat : public TypedDiskFormat< TextureMap< T > >,
                             public Singleton< TextureMapDiskFormat< T > >
{

  public:

    ~TextureMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name, TextureMap< T >& object ) const;
    void write( const std::string& name,
                TextureMap< T >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< TextureMapDiskFormat< T > >;

    TextureMapDiskFormat();

};


}


//
// forcing instanciation of some default TextureMap disk formats is useless
// since there can be a large set of types for template parameter T;
// so, here is an example of singleton instanciation and disk format 
// registration that can be used as a model; here, we designed a texture map of
// QBall items
//
//
// #include <gkg-core-io/DiskFormatFactory_i.h>
// #include <gkg-processing-container/TextureMap_i.h>
// #include <gkg-processing-io/SiteMapDiskFormat_i.h>
// #include <gkg-core-exception/Exception.h>
//
//
// namespace gkg
// {
//
// template <>
// gkg::DiskFormatFactory< gkg::TextureMap< gkg::QBallModel > >*
//    gkg::Singleton< gkg::DiskFormatFactory<
//                      gkg::TextureMap< gkg::QBallModel > >
//                  >::_instance = 0;
//
// }
//
// namespace gkg
// {
//
// template <>
// gkg::TextureMapDiskFormat< gkg::QBallModel >*
//    gkg::Singleton< gkg::TextureMapDiskFormat< gkg::QBallModel >
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
//     gkg::DiskFormatFactory< gkg::TextureMap< gkg::QBallModel >
//                           >::getInstance().registerDiskFormat(
//       gkg::TextureMapDiskFormat< gkg::QBallModel >::getInstance().getName(),
//       &gkg::TextureMapDiskFormat< gkg::QBallModel >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "TextureMap disk format registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterTextureDiskFormat( TYPE, NAME )                       \
static bool initializeDiskFormat##NAME()                              \
{                                                                     \
                                                                      \
  try                                                                 \
  {                                                                   \
                                                                      \
    gkg::DiskFormatFactory< gkg::TextureMap< TYPE >                   \
                          >::getInstance().registerDiskFormat(        \
      gkg::TextureMapDiskFormat< TYPE                                 \
                               >::getInstance().getName(),            \
      &gkg::TextureMapDiskFormat< TYPE >::getInstance() );            \
                                                                      \
    return true;                                                      \
                                                                      \
  }                                                                   \
  GKG_CATCH( "TextureMap disk format registration: " )                \
                                                                      \
}                                                                     \
                                                                      \
static bool initializedDiskFormat##NAME __attribute__((unused)) =     \
  initializeDiskFormat##NAME()


#endif
