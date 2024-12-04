#ifndef _gkg_dmri_io_BundleMeasureSpreadSheetDiskFormat_h_
#define _gkg_dmri_io_BundleMeasureSpreadSheetDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class L, class Compare > class BundleMeasureSpreadSheet;


template < class L, class Compare = std::less< L > >
class BundleMeasureSpreadSheetDiskFormat :
            public TypedDiskFormat< BundleMeasureSpreadSheet< L, Compare > >,
            public Singleton< BundleMeasureSpreadSheetDiskFormat< L, Compare > >
{

  public:

    ~BundleMeasureSpreadSheetDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               BundleMeasureSpreadSheet< L, Compare >& object ) const;
    void write( const std::string& name,
                BundleMeasureSpreadSheet< L, Compare >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< BundleMeasureSpreadSheetDiskFormat< L, Compare > >;

    BundleMeasureSpreadSheetDiskFormat();

};


}


//
// forcing instanciation of all the BundleMeasureSpreadSheet disk formats 
// is useless since there can be a large set of types for template parameter L;
// we have only instanciated some default integer and string disk formats
// in BundleMeasureSpreadSheetDiskFormat.cxx;
// so, here is an example of singleton instanciation and disk format 
// registration that can be used as a model; here, we designed a fiber measure
// spread sheet of int32_t items
//
//
// #include <gkg-core-io/DiskFormatFactory_i.h>
// #include <gkg-processing-container/BundleMeasureSpreadSheet_i.h>
// #include <gkg-core-exception/Exception.h>
//
//
// namespace gkg
// {
//
// template <>
// gkg::DiskFormatFactory< gkg::BundleMeasureSpreadSheet< int32_t > >*
//    gkg::Singleton< gkg::DiskFormatFactory<
//                      gkg::BundleMeasureSpreadSheet< int32_t > >
//                  >::_instance = 0;
//
// }
//
// namespace gkg
// {
//
// template <>
// gkg::BundleMeasureSpreadSheetDiskFormat< int32_t >*
//    gkg::Singleton< gkg::BundleMeasureSpreadSheetDiskFormat< int32_t > >::
//                                                                _instance = 0;
//
// }
//
// static bool initialize()
// {
//
//   try
//   {
//
//     gkg::DiskFormatFactory< gkg::BundleMeasureSpreadSheet< int32_t >
//                           >::getInstance().registerDiskFormat(
//       gkg::BundleMeasureSpreadSheetDiskFormat< int32_t >::getInstance().
//                                                                    getName(),
//       &gkg::BundleMeasureSpreadSheetDiskFormat< int32_t >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "BundleMeasureSpreadSheet disk format registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterBundleMeasureSpreadSheetDiskFormat( LABEL, COMPARE, NAME ) \
static bool initializeDiskFormat##NAME()                              \
{                                                                     \
                                                                      \
  try                                                                 \
  {                                                                   \
                                                                      \
    gkg::DiskFormatFactory< gkg::BundleMeasureSpreadSheet< LABEL,     \
                                                          COMPARE >   \
                          >::getInstance().registerDiskFormat(        \
      gkg::BundleMeasureSpreadSheetDiskFormat< LABEL, COMPARE         \
                               >::getInstance().getName(),            \
      &gkg::BundleMeasureSpreadSheetDiskFormat< LABEL, COMPARE >::    \
                                                   getInstance() );   \
                                                                      \
    return true;                                                      \
                                                                      \
  }                                                                   \
  GKG_CATCH( "BundleMeasureSpreadSheet disk format registration: " )  \
                                                                      \
}                                                                     \
                                                                      \
static bool initializedDiskFormat##NAME = initializeDiskFormat##NAME()


#endif
