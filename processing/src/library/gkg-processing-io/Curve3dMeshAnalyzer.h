#ifndef _gkg_processing_io_Curve3dMeshAnalyzer_h_
#define _gkg_processing_io_Curve3dMeshAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class Curve3dMeshAnalyzer : public BaseDiskFormatAnalyzer,
                          public Singleton< Curve3dMeshAnalyzer< T > >
{

  public:

    ~Curve3dMeshAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< Curve3dMeshAnalyzer< T > >;

    Curve3dMeshAnalyzer();

};


}


#endif

