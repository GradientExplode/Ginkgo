#ifndef _gkg_mri_reconstruction_io_GehcSignaPFileDiskFormat_h_
#define _gkg_mri_reconstruction_io_GehcSignaPFileDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/Indent.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;
template < class T > class VolumeProxy;
class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;


template < class T >
class GehcSignaPFileDiskFormat :
                          public TypedDiskFormat< Volume< std::complex< T > > >,
                          public Singleton< GehcSignaPFileDiskFormat< T > >
{

  public:

    ~GehcSignaPFileDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;
    void read( const std::string& name,
               Volume< std::complex< T > >& object ) const;

    void preparePartialBinaryRead( const std::string& name,
                                   HeaderedObject& proxy ) const;
    void partialBinaryRead( const HeaderedObject& proxy,
                            GenericObject& subpart,
                            const RegionInformation& regionInformation ) const;

    bool hasReader() const;

    void setLogStream( std::ostream& logStream );

  protected:

    friend class Singleton< GehcSignaPFileDiskFormat< T > >;

    GehcSignaPFileDiskFormat();

    void getBSwapAndRevision( std::istream& is,
                              bool& bswap,
                              float& revision ) const;
    size_t getSliceFactor( float revision ) const;
    size_t getRdbHeaderRecSize( float revision ) const;
    size_t getRdbPerPassTabSize( float revision ) const;
    size_t getRdbNexTypeSize( float revision ) const;
    size_t getToolsDataSize( float revision ) const;
    size_t getExamDataTypeSize( float revision ) const;
    size_t getSeriesDataTypeSize( float revision ) const;
    size_t getMrImageDataTypeSize( float revision ) const;
    size_t getPoolHeaderSize( float revision ) const;
    void skipPadding( std::istream& is,
                      off_t start,
                      size_t size ) const;

    void readPartialBinaryCore(
                            std::istream& is,
                            const ItemReader< int16_t >& itemR,
                            const VolumeProxy< std::complex< T > >& volumeProxy,
                            Volume< std::complex< T > >& subVolume,
                            const BoundingBox< int32_t >& boundingBox,
                            float revision ) const;

    // RDBM_HEADER
    void readRdbHeaderRec( std::istream& is,
                           const std::string& prefix,
                           HeaderedObject& object,
                           bool bswap,
                           float revision ) const;

    void readRdbPerPassInfoEntryTab( std::istream& is, 
                                     const std::string& prefix,
                                     HeaderedObject& object,
                                     bool bswap,
                                     float revision ) const;

    void readRdbSliceInfoEntryTab( std::istream& is, 
                                   const std::string& prefix,
                                   HeaderedObject& object,
                                   bool bswap,
                                   float revision ) const;

    void readRdbNexType( std::istream& is, 
                         const std::string& prefix,
                         HeaderedObject& object,
                         bool bswap,
                         float revision ) const;

    // IMAGE_HEADER
    void readExamDataType( std::istream& is, 
                           const std::string& prefix,
                           HeaderedObject& object,
                           bool bswap,
                           float revision ) const;
    void readSeriesDataType( std::istream& is, 
                             const std::string& prefix,
                             HeaderedObject& object,
                             bool bswap,
                             float revision ) const;
    void readMrImageDataType( std::istream& is, 
                              const std::string& prefix,
                              HeaderedObject& object,
                              bool bswap,
                              float revision ) const;

    // low-level read funtion(s)
    template < class U >
    void readItem( std::istream& is,
                   const ItemReader< U >& itemR,
                   const std::string& prefix,
                   const std::string& name,
                   HeaderedObject& object,
                   float revision = 0.0,
                   int32_t revisionMask = 0 ) const;
    void readStringItem( std::istream& is,
                         const ItemReader< int8_t >& itemR,
                         size_t count,
                         const std::string& prefix,
                         const std::string& name,
                         HeaderedObject& object,
                         float revision = 0.0,
                         int32_t revisionMask = 0 ) const;
    template < class U >
    void readListItem( std::istream& is,
                       const ItemReader< U >& itemR,
                       size_t count,
                       const std::string& prefix,
                       const std::string& name,
                       HeaderedObject& object,
                       float revision = 0.0,
                       int32_t revisionMask = 0 ) const;

    void readVarTypeItem( std::istream& is, 
                          const ItemReader< int8_t >& itemR_CHAR,
                          const ItemReader< int32_t >& itemR_LONG,
                          const ItemReader< uint32_t >& itemR_ULONG,
                          const std::string& prefix,
                          const std::string& name,
                          gkg::HeaderedObject& object,
                          float revision = 0.0,
                          int32_t revisionMask = 0 ) const;

    std::ostream* _logStream;
    mutable Indent _indent;

};


}


#endif
