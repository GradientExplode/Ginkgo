#ifndef _gkg_mri_reconstruction_io_GehcSignaGenesis2DiskFormat_h_
#define _gkg_mri_reconstruction_io_GehcSignaGenesis2DiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;
template < class T > class VolumeProxy;
class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;


template < class T >
class GehcSignaGenesis2DiskFormat :
                            public TypedDiskFormat< Volume< T > >,
                            public Singleton< GehcSignaGenesis2DiskFormat< T > >
{

  public:

    ~GehcSignaGenesis2DiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;
    void read( const std::string& name,
               Volume< T >& object ) const;

    void preparePartialBinaryRead( const std::string& name,
                                   HeaderedObject& proxy ) const;
    void partialBinaryRead( const HeaderedObject& proxy,
                            GenericObject& subpart,
                            const RegionInformation& regionInformation ) const;

    bool hasReader() const;

  protected:

    friend class Singleton< GehcSignaGenesis2DiskFormat< T > >;

    struct SliceInfo
    {

      int16_t  im_no;
      float    loc;
      float    ctr_R;
      float    ctr_A;
      float    ctr_S;
      float    tlhc_R;
      float    tlhc_A;
      float    tlhc_S;
      float    trhc_R;
      float    trhc_A;
      float    trhc_S;
      float    brhc_R;
      float    brhc_A;
      float    brhc_S;
      int32_t  rawrunnum;
      uint32_t im_checksum;
      int32_t  img_dwindow;
      int32_t  img_dlevel;

    };

    GehcSignaGenesis2DiskFormat();

    void getBSwapAndRevisionAndOffsets(
                              const std::string& name,
                              bool& bswap,
                              float& revision,
                              uint32_t& pixHdrOffset,
                              uint32_t& suiteDataTypeOffset,
                              uint32_t& examDataTypeOffset,
                              uint32_t& seriesDataTypeOffset,
                              uint32_t& mrImageDataTypeOffset ) const;

    void readCoreHeader( const std::string& name,
                         HeaderedObject& object,
                         SliceInfo& sliceInfo,
                         bool bswap,
                         float revision,
                         uint32_t pixHdrOffset,
                         uint32_t suiteDataTypeOffset,
                         uint32_t examDataTypeOffset,
                         uint32_t seriesDataTypeOffset,
                         uint32_t mrImageDataTypeOffset ) const;
    void readPartialBinaryCore( const ItemReader< T >& itemR,
                                const VolumeProxy< T >& volumeProxy,
                                Volume< T >& subVolume,
                                const BoundingBox< int32_t >& boundingBox,
                                float revision ) const;

    // GENESIS_HEADER
    void readPixHdr( std::istream& is, 
                     const std::string& prefix,
                     HeaderedObject& object,
                     SliceInfo& sliceInfo,
                     bool bswap,
                     float revision,
                     uint32_t pixHdrOffset ) const;
    void readSuiteDataType( std::istream& is, 
                            const std::string& prefix,
                            HeaderedObject& object,
                            bool bswap,
                            float revision,
                            uint32_t suiteDataTypeOffset ) const;
    void readExamDataType( std::istream& is, 
                           const std::string& prefix,
                           HeaderedObject& object,
                           bool bswap,
                           float revision,
                           uint32_t examDataTypeOffset ) const;
    void readSeriesDataType( std::istream& is, 
                             const std::string& prefix,
                             HeaderedObject& object,
                             bool bswap,
                             float revision,
                             uint32_t seriesDataTypeOffset ) const;
    void readMrImageDataType( std::istream& is, 
                              const std::string& prefix,
                              HeaderedObject& object,
                              SliceInfo& sliceInfo,
                              bool bswap,
                              float revision,
                              uint32_t mrImageDataTypeOffset ) const;

    // low-level read funtion(s)
    template < class U >
    void readBaseItem( std::istream& is,
                       const ItemReader< U >& itemR,
                       U& u,
                       float revision = 0.0,
                       int32_t revisionMask = 0 ) const;
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
    void readVarTypeItem( std::istream& is, 
                          const ItemReader< int8_t >& itemR_CHAR,
                          const ItemReader< int32_t >& itemR_LONG,
                          const ItemReader< uint32_t >& itemR_ULONG,
                          const std::string& prefix,
                          const std::string& name,
                          gkg::HeaderedObject& object,
                          float revision = 0.0,
                          int32_t revisionMask = 0 ) const;

};


}


#endif
