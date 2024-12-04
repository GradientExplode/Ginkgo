#ifndef _gkg_processing_io_AimsTextureAnalyzer_h_
#define _gkg_processing_io_AimsTextureAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class AimsTextureAnalyzer : public BaseDiskFormatAnalyzer,
                            public Singleton< AimsTextureAnalyzer< T > >
{

  public:

    ~AimsTextureAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< AimsTextureAnalyzer< T > >;

    AimsTextureAnalyzer();

};


}


#endif

