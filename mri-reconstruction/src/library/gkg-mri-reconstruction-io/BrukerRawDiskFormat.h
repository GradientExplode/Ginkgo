#ifndef _gkg_mri_reconstruction_io_BrukerRawDiskFormat_h_
#define _gkg_mri_reconstruction_io_BrukerRawDiskFormat_h_


#include <gkg-core-cppext/LargeFileStream.h>
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
class BrukerRawDiskFormat :
                          public TypedDiskFormat< Volume< std::complex< T > > >,
                          public Singleton< BrukerRawDiskFormat< T > >
{

  public:

    ~BrukerRawDiskFormat();

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

  protected:

    friend class Singleton< BrukerRawDiskFormat< T > >;

    BrukerRawDiskFormat();

    void readPartialBinaryCore(
                            gkg::largefile_ifstream& is,
                            const ItemReader< int32_t >& itemR,
                            const VolumeProxy< std::complex< T > >& volumeProxy,
                            Volume< std::complex< T > >& subVolume,
                            const BoundingBox< int32_t >& boundingBox ) const;

    // header function(s)
    void readAcqpHeader( std::istream& is,
                         HeaderedObject& object ) const;

    void readImndHeader( std::istream& is, 
                         HeaderedObject& object ) const;

    void readMethodHeader( std::istream& is, 
                           HeaderedObject& object ) const;


    // low-level read funtion(s)
    void replaceDiezeWithUnderscore( std::string& data ) const;
    void readInteger( std::istream& is,
                      const std::string& attributeName,
                      gkg::HeaderedObject& object ) const;
    void readDouble( std::istream& is,
                     const std::string& attributeName,
                     gkg::HeaderedObject& object ) const;
    void readString( std::istream& is,
                     const std::string& attributeName,
                     gkg::HeaderedObject& object ) const;
    void readEnum( std::istream& is,
                   const std::string& attributeName,
                   gkg::HeaderedObject& object ) const;
    void readStructure( std::istream& is,
                        const std::string& attributeName,
                        gkg::HeaderedObject& object ) const;

    void readArrayInteger( std::istream& is,
                           const std::string& attributeName,
                           gkg::HeaderedObject& object ) const;
    void readArrayDouble( std::istream& is,
                          const std::string& attributeName,
                          gkg::HeaderedObject& object ) const;
    void readArrayString( std::istream& is,
                          const std::string& attributeName,
                          gkg::HeaderedObject& object ) const;
    void readArrayEnum( std::istream& is,
                        const std::string& attributeName,
                        gkg::HeaderedObject& object ) const;
    void readArrayStructure( std::istream& is,
                             const std::string& attributeName,
                             gkg::HeaderedObject& object ) const;

    std::vector< int32_t > readArraySize( std::istream& is ) const;
    std::vector< std::string > readStructureItems( std::istream& is ) const;

    bool findPosition( std::istream& is,
                       const std::string& attributeName ) const;
    void skipWhiteSpace( std::istream& is ) const;

};


}


#endif
