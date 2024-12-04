#ifndef _gkg_processing_io_SiteMapAnalyzer_h_
#define _gkg_processing_io_SiteMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class R, class S, class Compare = std::less< R > >
class SiteMapAnalyzer : public BaseDiskFormatAnalyzer,
                        public Singleton< SiteMapAnalyzer< R, S, Compare > >
{

  public:

    ~SiteMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< SiteMapAnalyzer< R, S, Compare > >;

    SiteMapAnalyzer();

};


}


#endif

