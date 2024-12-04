#ifndef _gkg_processing_io_GisDiskFormat_h_
#define _gkg_processing_io_GisDiskFormat_h_


#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class VolumeProxy;
class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;
template < class T > class ItemWriter;


template < class T >
class GisDiskFormat : public TypedDiskFormat< Volume< T > >,
                      public Singleton< GisDiskFormat< T > >
{

  public:

    ~GisDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               Volume< T >& object ) const;
    void write( const std::string& name,
                Volume< T >& object,
                bool ascii ) const;

    void preparePartialBinaryRead( const std::string& name,
                                   HeaderedObject& proxy ) const;
    void partialBinaryRead( const HeaderedObject& proxy,
                            GenericObject& subpart,
                            const RegionInformation& regionInformation ) const;

    void preparePartialBinaryWrite( const std::string& name,
                                    HeaderedObject& proxy ) const;
    void partialBinaryWrite( const HeaderedObject& proxy,
                             const GenericObject& subpart,
                             const RegionInformation& regionInformation ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

    std::string getTypeName() const;

  protected:

    friend class Singleton< GisDiskFormat< T > >;

    GisDiskFormat();

    void writeCore( gkg::largefile_ofstream& os,
                    const ItemWriter< T >& itemW,
                    const Volume< T >& object ) const;

    void readPartialBinaryCore( gkg::largefile_ifstream& is,
                                const ItemReader< T >& itemR,
                                const BoundingBox< int32_t >& boundingBox,
                                int32_t onDiskSizeX,
                                int32_t onDiskSizeXY,
                                int32_t onDiskSizeXYZ,
                                Volume< T >& subVolume ) const;
    void writePartialBinaryCore( gkg::largefile_fstream& fs,
                                 const ItemWriter< T >& itemW,
                                 const BoundingBox< int32_t >& boundingBox,
                                 int32_t onDiskSizeX,
                                 int32_t onDiskSizeXY,
                                 int32_t onDiskSizeXYZ,
                                 const Volume< T >& subVolume ) const;

};


}


#endif
