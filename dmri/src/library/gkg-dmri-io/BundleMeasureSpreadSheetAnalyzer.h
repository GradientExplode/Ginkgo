#ifndef _gkg_dmri_io_BundleMeasureSpreadSheetAnalyzer_h_
#define _gkg_dmri_io_BundleMeasureSpreadSheetAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMeasureSpreadSheetAnalyzer :
              public BaseDiskFormatAnalyzer,
              public Singleton< BundleMeasureSpreadSheetAnalyzer< L, Compare > >
{

  public:

    ~BundleMeasureSpreadSheetAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< BundleMeasureSpreadSheetAnalyzer< L, Compare > >;

    BundleMeasureSpreadSheetAnalyzer();

};


}


//
// forcing instanciation of all the FiberMeasureSpreadSheet analyzers is useless
// since there can be a large set of types for template parameter L;
// we have only instanciated some default integer and string analyzers
// in BundleMeasureSpreadSheetAnalyzer.cxx;
// so, here is an example of singleton instanciation and analyzer registration
// that can be used as a model; here, we designed a fiber measure spreadsheet
// of int32_t labels
//
//
// #include <gkg-processing-io/BundleMeasureSpreadSheetAnalyzer_i.h>
// #include <gkg-core-exception/Exception.h>
//
// namespace gkg
// {
//
// template <>
// gkg::BundleMeasureSpreadSheetAnalyzer< int32_t >*
//    gkg::Singleton< gkg::BundleMeasureSpreadSheetAnalyzer< int32_t > >::
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
//     gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
//       gkg::FiberMeasureSpreadSheetDiskFormat< int32_t >::getInstance().
//                                                                    getName(),
//       &gkg::BundleMeasureSpreadSheetAnalyzer< int32_t >::getInstance() );
//
//     return true;
//
//   }
//   GKG_CATCH( "Fiber Measure Spread Sheet analyzer registration: " )
//
// }
//
// static bool initialized = initialize();
//


#define RegisterBundleMeasureSpreadSheetAnalyzer( LABEL, COMPARE, NAME )    \
static bool initializeAnalyzer##NAME()                                     \
{                                                                          \
                                                                           \
  try                                                                      \
  {                                                                        \
                                                                           \
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(               \
      gkg::FiberMeasureSpreadSheetDiskFormat< LABEL, COMPARE >::           \
                                                  getInstance().getName(), \
      &gkg::BundleMeasureSpreadSheetAnalyzer< LABEL, COMPARE >::            \
                                                 getInstance() );          \
                                                                           \
    return true;                                                           \
                                                                           \
  }                                                                        \
  GKG_CATCH( "Fiber Measure Spread Sheet analyzer registration: " )        \
                                                                           \
}                                                                          \
                                                                           \
static bool initializedAnalyzer##NAME = initializeAnalyzer##NAME()


#endif

