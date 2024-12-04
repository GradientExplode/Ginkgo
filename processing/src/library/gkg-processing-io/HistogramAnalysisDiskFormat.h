#ifndef _gkg_processing_io_HistogramAnalysisDiskFormat_h_
#define _gkg_processing_io_HistogramAnalysisDiskFormat_h_

#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class HistogramAnalysis;


class HistogramAnalysisDiskFormat :
                                 public TypedDiskFormat< HistogramAnalysis >,
                                 public Singleton< HistogramAnalysisDiskFormat >
{

  public:
  
    ~HistogramAnalysisDiskFormat();
    
    std::string getName() const;
    
    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;


    void read( const std::string& name,
               HistogramAnalysis& object ) const;
    void write( const std::string& name,
                HistogramAnalysis& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< HistogramAnalysisDiskFormat >;

    HistogramAnalysisDiskFormat();
    
    void readHANFile( const std::string& hanName,
                      HistogramAnalysis& object  ) const;

};


}


#endif
