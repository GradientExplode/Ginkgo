#ifndef _gkg_processing_io_StlMeshAnalyzer_h_
#define _gkg_processing_io_StlMeshAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class StlMeshAnalyzer : public BaseDiskFormatAnalyzer,
                        public Singleton< StlMeshAnalyzer >
{

  public:

    ~StlMeshAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< StlMeshAnalyzer >;

    StlMeshAnalyzer();

};


}


#endif

