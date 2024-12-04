#ifndef _gkg_mri_reconstruction_io_GehcSignaGenesisAnalyzer_h_
#define _gkg_mri_reconstruction_io_GehcSignaGenesisAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class GehcSignaGenesisAnalyzer : 
                               public BaseDiskFormatAnalyzer,
                               public Singleton< GehcSignaGenesisAnalyzer< T > >
{

  public:

    ~GehcSignaGenesisAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;

  private:

    friend class Singleton< GehcSignaGenesisAnalyzer< T > >;

    GehcSignaGenesisAnalyzer();

};


}


#endif

