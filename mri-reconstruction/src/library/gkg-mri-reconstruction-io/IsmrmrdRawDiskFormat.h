#ifndef _gkg_mri_reconstruction_io_IsmrmrdRawDiskFormat_h_
#define _gkg_mri_reconstruction_io_IsmrmrdRawDiskFormat_h_


#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <ismrmrd/dataset.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;
template < class T > class VolumeProxy;
class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;


template < class T >
class IsmrmrdRawDiskFormat :
                          public TypedDiskFormat< Volume< std::complex< T > > >,
                          public Singleton< IsmrmrdRawDiskFormat< T > >
{

  public:

    ~IsmrmrdRawDiskFormat();

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

    friend class Singleton< IsmrmrdRawDiskFormat< T > >;

    IsmrmrdRawDiskFormat();

    void readPartialBinaryCore(
                            ISMRMRD::Dataset& is,
                            const VolumeProxy< std::complex< T > >& volumeProxy,
                            Volume< std::complex< T > >& subVolume,
                            const BoundingBox< int32_t >& boundingBox ) const;

};


}


#endif
