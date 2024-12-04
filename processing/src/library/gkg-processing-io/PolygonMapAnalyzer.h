#ifndef _gkg_processing_io_PolygonMapAnalyzer_h_
#define _gkg_processing_io_PolygonMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < uint32_t D >
class PolygonMapAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< PolygonMapAnalyzer< D > >
{

  public:

    ~PolygonMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< PolygonMapAnalyzer< D > >;

    PolygonMapAnalyzer();

};


}


#endif

