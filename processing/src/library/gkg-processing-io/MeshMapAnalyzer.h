#ifndef _gkg_processing_io_MeshMapAnalyzer_h_
#define _gkg_processing_io_MeshMapAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshMapAnalyzer : public BaseDiskFormatAnalyzer,
                        public Singleton< MeshMapAnalyzer< R, S, D, Compare > >
{

  public:

    ~MeshMapAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< MeshMapAnalyzer< R, S, D, Compare > >;

    MeshMapAnalyzer();

};


}


#endif

