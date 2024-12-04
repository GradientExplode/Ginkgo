#ifndef _gkg_mri_reconstruction_io_BrukerRawAnalyzer_h_
#define _gkg_mri_reconstruction_io_BrukerRawAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class BrukerRawAnalyzer : public BaseDiskFormatAnalyzer,
                          public Singleton< BrukerRawAnalyzer< T > >
{

  public:

    ~BrukerRawAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< BrukerRawAnalyzer< T > >;

    BrukerRawAnalyzer();

};


}


#endif

