#ifndef _gkg_mri_reconstruction_io_GehcSignaGenesis2Analyzer_h_
#define _gkg_mri_reconstruction_io_GehcSignaGenesis2Analyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class GehcSignaGenesis2Analyzer : 
                               public BaseDiskFormatAnalyzer,
                               public Singleton< GehcSignaGenesis2Analyzer< T > >
{

  public:

    ~GehcSignaGenesis2Analyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;

  private:

    friend class Singleton< GehcSignaGenesis2Analyzer< T > >;

    GehcSignaGenesis2Analyzer();

};


}


#endif

