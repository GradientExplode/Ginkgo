#ifndef _gkg_processing_plugin_nifti_NiftiAnalyzer_h_
#define _gkg_processing_plugin_nifti_NiftiAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class NiftiAnalyzer : public BaseDiskFormatAnalyzer,
                      public Singleton< NiftiAnalyzer< T > >
{

  public:

    ~NiftiAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;

  private:

    friend class Singleton< NiftiAnalyzer< T > >;

    NiftiAnalyzer();

};


}


#endif

