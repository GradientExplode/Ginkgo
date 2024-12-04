#ifndef _gkg_fmri_io_ParadigmAnalyzer_h_
#define _gkg_fmri_io_ParadigmAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class ParadigmAnalyzer : public BaseDiskFormatAnalyzer,
                         public Singleton< ParadigmAnalyzer >
{

  public:

    ~ParadigmAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< ParadigmAnalyzer >;

    ParadigmAnalyzer();

};


}


#endif

