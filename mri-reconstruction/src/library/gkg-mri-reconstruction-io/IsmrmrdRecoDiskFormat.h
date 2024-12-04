#ifndef _gkg_mri_reconstruction_io_IsmrmrdRecoDiskFormat_h_
#define _gkg_mri_reconstruction_io_IsmrmrdRecoDiskFormat_h_


#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <ismrmrd/dataset.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class VolumeProxy;
class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;


template < class T >
class IsmrmrdRecoDiskFormat : public TypedDiskFormat< Volume< T > >,
                              public Singleton< IsmrmrdRecoDiskFormat< T > >
{

  public:

    ~IsmrmrdRecoDiskFormat();

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

    friend class Singleton< IsmrmrdRecoDiskFormat< T > >;

    static herr_t getFileInformation( hid_t loc_id,
                                      const char *name,
                                      void* opdata );

    IsmrmrdRecoDiskFormat();

    void readPartialBinaryCore(
                              ISMRMRD::Dataset& is,
                              const VolumeProxy< T >& volumeProxy,
                              Volume< T >& subVolume,
                              const BoundingBox< int32_t >& boundingBox ) const;

};


}


#endif
