#ifndef _gkg_dmri_io_BundleMapAnalyzer_h_
#define _gkg_dmri_io_BundleMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMapAnalyzer : public BaseDiskFormatAnalyzer,
                          public Singleton< BundleMapAnalyzer< L, Compare > >
{

  public:

    ~BundleMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< BundleMapAnalyzer< L, Compare > >;

    BundleMapAnalyzer();

};


}


//
// forcing instanciation of all the BundleMap analyzers is useless
// since there can be a large set of types for template parameter L;
// we have only instanciated some default integer and string analyzers
// in BundleMapAnalyzer.cxx;
// so, here is an example of singleton instanciation and analyzer registration
// that can be used as a model; here, we designed a bundle map of int32_t labels
//
//
// #include <gkg-processing-io/BundleMapAnalyzer_i.h>
// #include <gkg-core-exception/Exception.h>
//
// namespace gkg
// {
//
// template <>
// gkg::BundleMapAnalyzer< int32_t >*
//    gkg::Singleton< gkg::BundleMapAnalyzer< int32_t > >::_instance = 0;
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
//       gkg::BundleMapDiskFormat< int32_t >::getInstance().getName(),
//       &gkg::BundleMapAnalyzer< int32_t >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "Bundle Map analyzer registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterBundleMapAnalyzer( LABEL, COMPARE, NAME )                  \
static bool initializeAnalyzer##NAME()                                     \
{                                                                          \
                                                                           \
  try                                                                      \
  {                                                                        \
                                                                           \
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(               \
      gkg::BundleMapDiskFormat< LABEL, COMPARE >::getInstance().getName(), \
      &gkg::BundleMapAnalyzer< LABEL, COMPARE >::getInstance() );          \
                                                                           \
    return true;                                                           \
                                                                           \
  }                                                                        \
  GKG_CATCH( "Bundle Map analyzer registration: " )                        \
                                                                           \
}                                                                          \
                                                                           \
static bool initializedAnalyzer##NAME = initializeAnalyzer##NAME()


#endif

