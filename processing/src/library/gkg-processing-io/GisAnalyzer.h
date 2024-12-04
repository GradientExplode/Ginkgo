#ifndef _gkg_processing_io_GisAnalyzer_h_
#define _gkg_processing_io_GisAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>


namespace gkg
{


template < class T >
class GisAnalyzer : public BaseDiskFormatAnalyzer,
                    public Singleton< GisAnalyzer< T > >
{

  public:

    ~GisAnalyzer();

    DiskFormat& getDiskFormat() const;
    void analyze( const std::string& name,
                  std::string& format,
                  gkg::AnalyzedObject& analyzedObject ) const;


  private:

    friend class Singleton< GisAnalyzer< T > >;

    GisAnalyzer();

};


}


#endif

