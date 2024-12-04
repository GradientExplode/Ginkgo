#ifndef _gkg_simulation_io_MedusaGeometryAnalyzer_h_
#define _gkg_simulation_io_MedusaGeometryAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class MedusaGeometryAnalyzer : public BaseDiskFormatAnalyzer,
                               public Singleton< MedusaGeometryAnalyzer >
{

  public:

    ~MedusaGeometryAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< MedusaGeometryAnalyzer >;

    MedusaGeometryAnalyzer();

};


}


#endif

