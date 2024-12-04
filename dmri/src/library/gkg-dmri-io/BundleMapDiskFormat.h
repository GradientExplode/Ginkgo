#ifndef _gkg_dmri_io_BundleMapDiskFormat_h_
#define _gkg_dmri_io_BundleMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class L, class Compare > class BundleMap;


template < class L, class Compare = std::less< L > >
class BundleMapDiskFormat : public TypedDiskFormat< BundleMap< L, Compare > >,
                            public Singleton< BundleMapDiskFormat< L, Compare> >
{

  public:

    ~BundleMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name, BundleMap< L, Compare >& object ) const;
    void write( const std::string& name,
                BundleMap< L, Compare >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< BundleMapDiskFormat< L, Compare > >;

    BundleMapDiskFormat();

};


}


//
// forcing instanciation of all the BundleMap disk formats is useless
// since there can be a large set of types for template parameter L;
// we have only instanciated some default integer and string disk formats
// in BundleMapDiskFormat.cxx;
// so, here is an example of singleton instanciation and disk format 
// registration that can be used as a model; here, we designed a texture map of
// int32_t items
//
//
// #include <gkg-core-io/DiskFormatFactory_i.h>
// #include <gkg-processing-container/BundleMap_i.h>
// #include <gkg-processing-io/SiteMapDiskFormat_i.h>
// #include <gkg-core-exception/Exception.h>
//
//
// namespace gkg
// {
//
// template <>
// gkg::DiskFormatFactory< gkg::BundleMap< int32_t > >*
//    gkg::Singleton< gkg::DiskFormatFactory<
//                      gkg::BundleMap< int32_t > >
//                  >::_instance = 0;
//
// }
//
// namespace gkg
// {
//
// template <>
// gkg::BundleMapDiskFormat< int32_t >*
//    gkg::Singleton< gkg::BundleMapDiskFormat< int32_t > >::_instance = 0;
//
// }
//
// static bool initialize()
// {
//
//   try
//   {
//
//     gkg::DiskFormatFactory< gkg::BundleMap< int32_t >
//                           >::getInstance().registerDiskFormat(
//       gkg::BundleMapDiskFormat< int32_t >::getInstance().getName(),
//       &gkg::BundleMapDiskFormat< int32_t >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "BundleMap disk format registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterBundleMapDiskFormat( LABEL, COMPARE, NAME )           \
static bool initializeDiskFormat##NAME()                              \
{                                                                     \
                                                                      \
  try                                                                 \
  {                                                                   \
                                                                      \
    gkg::DiskFormatFactory< gkg::BundleMap< LABEL, COMPARE >          \
                          >::getInstance().registerDiskFormat(        \
      gkg::BundleMapDiskFormat< LABEL, COMPARE                        \
                               >::getInstance().getName(),            \
      &gkg::BundleMapDiskFormat< LABEL, COMPARE >::getInstance() );   \
                                                                      \
    return true;                                                      \
                                                                      \
  }                                                                   \
  GKG_CATCH( "BundleMap disk format registration: " )                 \
                                                                      \
}                                                                     \
                                                                      \
static bool initializedDiskFormat##NAME = initializeDiskFormat##NAME()


#endif
