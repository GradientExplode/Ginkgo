#ifndef _gkg_processing_io_AimsAcPcIhAnalyzer_h_
#define _gkg_processing_io_AimsAcPcIhAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class AimsAcPcIhAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< AimsAcPcIhAnalyzer >
{

  public:

    ~AimsAcPcIhAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< AimsAcPcIhAnalyzer >;

    AimsAcPcIhAnalyzer();

};


}


#endif

