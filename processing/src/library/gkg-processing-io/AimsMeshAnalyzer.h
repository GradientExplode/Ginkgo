#ifndef _gkg_processing_io_AimsMeshAnalyzer_h_
#define _gkg_processing_io_AimsMeshAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < uint32_t D >
class AimsMeshAnalyzer : public BaseDiskFormatAnalyzer,
                         public Singleton< AimsMeshAnalyzer< D > >
{

  public:

    ~AimsMeshAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< AimsMeshAnalyzer< D > >;

    AimsMeshAnalyzer();

};


}


#endif

