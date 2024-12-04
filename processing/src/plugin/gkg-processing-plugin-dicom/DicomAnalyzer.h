#ifndef _gkg_processing_plugin_dicom_DicomAnalyzer_h_
#define _gkg_processing_plugin_dicom_DicomAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class DicomAnalyzer : public BaseDiskFormatAnalyzer,
                      public Singleton< DicomAnalyzer< T > >
{

  public:

    ~DicomAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;

  private:

    friend class Singleton< DicomAnalyzer< T > >;

    DicomAnalyzer();

};


}


#endif

