#ifndef _gkg_processing_io_AimsTextureDiskFormat_h_
#define _gkg_processing_io_AimsTextureDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-container/TextureMap.h>
#include <vector>


namespace gkg
{


template < class T >
class AimsTextureDiskFormat : public TypedDiskFormat< TextureMap< T > >,
                              public Singleton< AimsTextureDiskFormat< T > >
{

  public:

    ~AimsTextureDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               TextureMap< T >& object ) const;
    void write( const std::string& name,
                TextureMap< T >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;
    std::string getTypeName() const;

  protected:

    friend class Singleton< AimsTextureDiskFormat< T > >;

    AimsTextureDiskFormat();

    int32_t getRankInformation( const HeaderedObject& object,
                                int32_t& rankCount,
                                std::vector< int32_t >& rank,
                                std::vector< int32_t >& itemCount ) const;


};


}


#endif
