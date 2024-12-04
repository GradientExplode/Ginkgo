#ifndef _gkg_processing_io_AimsBucketAnalyzer_h_
#define _gkg_processing_io_AimsBucketAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class AimsBucketAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< AimsBucketAnalyzer >
{

  public:

    ~AimsBucketAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< AimsBucketAnalyzer >;

    AimsBucketAnalyzer();

};


}


#endif

