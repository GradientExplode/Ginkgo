#ifndef _gkg_mri_reconstruction_io_IsmrmrdRecoAnalyzer_h_
#define _gkg_mri_reconstruction_io_IsmrmrdRecoAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class IsmrmrdRecoAnalyzer : public BaseDiskFormatAnalyzer,
                            public Singleton< IsmrmrdRecoAnalyzer< T > >
{

  public:

    ~IsmrmrdRecoAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< IsmrmrdRecoAnalyzer< T > >;

    IsmrmrdRecoAnalyzer();

};


}


#endif

