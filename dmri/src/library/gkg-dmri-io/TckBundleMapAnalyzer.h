#ifndef _gkg_dmri_io_TckBundleMapAnalyzer_h_
#define _gkg_dmri_io_TckBundleMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class TckBundleMapAnalyzer : public BaseDiskFormatAnalyzer,
                             public Singleton< TckBundleMapAnalyzer >
{

  public:

    ~TckBundleMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< TckBundleMapAnalyzer >;

    TckBundleMapAnalyzer();

};


}


#endif

