#ifndef _gkg_processing_io_Curve3dMapMeshAnalyzer_h_
#define _gkg_processing_io_Curve3dMapMeshAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class Curve3dMapMeshAnalyzer : public BaseDiskFormatAnalyzer,
                               public Singleton< Curve3dMapMeshAnalyzer< T > >
{

  public:

    ~Curve3dMapMeshAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< Curve3dMapMeshAnalyzer< T > >;

    Curve3dMapMeshAnalyzer();

};


}


#endif

