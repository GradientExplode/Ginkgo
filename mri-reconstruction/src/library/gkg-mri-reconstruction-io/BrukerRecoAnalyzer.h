#ifndef _gkg_mri_reconstruction_io_BrukerRecoAnalyzer_h_
#define _gkg_mri_reconstruction_io_BrukerRecoAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class BrukerRecoAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< BrukerRecoAnalyzer< T > >
{

  public:

    ~BrukerRecoAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< BrukerRecoAnalyzer< T > >;

    BrukerRecoAnalyzer();

};


}


#endif

