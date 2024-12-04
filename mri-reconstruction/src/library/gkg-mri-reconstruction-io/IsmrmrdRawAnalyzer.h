#ifndef _gkg_mri_reconstruction_io_IsmrmrdRawAnalyzer_h_
#define _gkg_mri_reconstruction_io_IsmrmrdRawAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class IsmrmrdRawAnalyzer : public BaseDiskFormatAnalyzer,
                           public Singleton< IsmrmrdRawAnalyzer< T > >
{

  public:

    ~IsmrmrdRawAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< IsmrmrdRawAnalyzer< T > >;

    IsmrmrdRawAnalyzer();

};


}


#endif

