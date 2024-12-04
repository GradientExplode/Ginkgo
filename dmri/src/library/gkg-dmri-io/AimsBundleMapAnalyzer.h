#ifndef _gkg_dmri_io_AimsBundleMapAnalyzer_h_
#define _gkg_dmri_io_AimsBundleMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class AimsBundleMapAnalyzer : public BaseDiskFormatAnalyzer,
                              public Singleton< AimsBundleMapAnalyzer >
{

  public:

    ~AimsBundleMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< AimsBundleMapAnalyzer >;

    AimsBundleMapAnalyzer();

};


}


#endif

