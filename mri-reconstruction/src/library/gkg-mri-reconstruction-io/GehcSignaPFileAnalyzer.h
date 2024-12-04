#ifndef _gkg_mri_reconstruction_io_GehcSignaPFileAnalyzer_h_
#define _gkg_mri_reconstruction_io_GehcSignaPFileAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class GehcSignaPFileAnalyzer : public BaseDiskFormatAnalyzer,
                               public Singleton< GehcSignaPFileAnalyzer< T > >
{

  public:

    ~GehcSignaPFileAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< GehcSignaPFileAnalyzer< T > >;

    GehcSignaPFileAnalyzer();

};


}


#endif

