#ifndef _gkg_processing_io_AimsAcPcIhDiskFormat_h_
#define _gkg_processing_io_AimsAcPcIhDiskFormat_h_

#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class AcPcIhCoordinates;


class AimsAcPcIhDiskFormat : public TypedDiskFormat< AcPcIhCoordinates >,
                             public Singleton< AimsAcPcIhDiskFormat >
{

  public:
  
    ~AimsAcPcIhDiskFormat();
    
    std::string getName() const;
    
    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;


    void read( const std::string& name,
               AcPcIhCoordinates& object ) const;
    void write( const std::string& name,
                AcPcIhCoordinates& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< AimsAcPcIhDiskFormat >;

    AimsAcPcIhDiskFormat();
    
    void readAPCFile( const std::string& apcName,
                      AcPcIhCoordinates& object  ) const;

};


}


#endif
