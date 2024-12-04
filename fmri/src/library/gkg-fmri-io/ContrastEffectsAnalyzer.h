#ifndef _gkg_fmri_io_ContrastEffectsAnalyzer_h_
#define _gkg_fmri_io_ContrastEffectsAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class ContrastEffectsAnalyzer : public BaseDiskFormatAnalyzer,
                                public Singleton< ContrastEffectsAnalyzer >
{

  public:

    ~ContrastEffectsAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< ContrastEffectsAnalyzer >;

    ContrastEffectsAnalyzer();

};


}


#endif

