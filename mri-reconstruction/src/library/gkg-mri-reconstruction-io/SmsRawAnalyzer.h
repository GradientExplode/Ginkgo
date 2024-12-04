#ifndef _gkg_mri_reconstruction_io_SmsRawAnalyzer_h_
#define _gkg_mri_reconstruction_io_SmsRawAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class SmsRawAnalyzer : public BaseDiskFormatAnalyzer,
                       public Singleton< SmsRawAnalyzer< T > >
{

  public:

    ~SmsRawAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< SmsRawAnalyzer< T > >;

    SmsRawAnalyzer();

};


}


#endif

