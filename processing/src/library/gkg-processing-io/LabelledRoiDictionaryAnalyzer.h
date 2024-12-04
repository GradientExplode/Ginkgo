#ifndef _gkg_processing_io_LabelledRoiDictionaryAnalyzer_h_
#define _gkg_processing_io_LabelledRoiDictionaryAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


class LabelledRoiDictionaryAnalyzer :
                               public BaseDiskFormatAnalyzer,
                               public Singleton< LabelledRoiDictionaryAnalyzer >
{

  public:

    ~LabelledRoiDictionaryAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< LabelledRoiDictionaryAnalyzer >;

    LabelledRoiDictionaryAnalyzer();

};


}


#endif

