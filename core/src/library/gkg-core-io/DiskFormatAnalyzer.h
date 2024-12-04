#ifndef _gkg_core_io_DiskFormatAnalyzer_h_
#define _gkg_core_io_DiskFormatAnalyzer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-object/HeaderedObject.h>
#include <string>
#include <map>


namespace gkg
{


class DiskFormat;


class AnalyzedObject : public HeaderedObject
{

  public:

    AnalyzedObject();
    ~AnalyzedObject();

    std::string getObjectType() const;
    std::string getItemType() const;
    std::string getRankType() const;
    std::string getLabelType() const;
    uint32_t getPolygonSize() const;

};


class BaseDiskFormatAnalyzer
{

  public:

    virtual ~BaseDiskFormatAnalyzer();

    virtual DiskFormat& getDiskFormat() const = 0;
    virtual void analyze( const std::string& name,
                          std::string& format,
                          AnalyzedObject& analyzedObject ) const = 0;

  protected:

    BaseDiskFormatAnalyzer();


};


class DiskFormatAnalyzer : public Singleton< DiskFormatAnalyzer >
{

  public:

    virtual ~DiskFormatAnalyzer();

    void analyze( const std::string& name,
                  std::string& format,
                  AnalyzedObject& analyzedObject,
                  const std::string& wantedObjectType = "" ) const;

    void registerAnalyzer( const std::string& format,
                           BaseDiskFormatAnalyzer* analyzer );

  private:

    friend class Singleton< DiskFormatAnalyzer >;

    DiskFormatAnalyzer();

    std::multimap< std::string, BaseDiskFormatAnalyzer* > _analyzers;

};


}


#endif
