#ifndef _gkg_mri_reconstrcution_io_SmsRawDiskFormat_h_
#define _gkg_mri_reconstrcution_io_SmsRawDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-mri-reconstruction-io/SmsRawHeader.h>
#include <gkg-core-io/Indent.h>
#include <gkg-processing-container/Volume.h>
#include <complex>


namespace gkg
{


class RegionInformation;
template < class T > class BoundingBox;
template < class T > class ItemReader;


template < class T >
class SmsRawDiskFormat : public TypedDiskFormat< Volume< std::complex< T > > >,
                         public Singleton< SmsRawDiskFormat< T > >
{

  public:

    ~SmsRawDiskFormat();

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

    friend class Singleton< SmsRawDiskFormat< T > >;

    SmsRawDiskFormat();

    std::string getType( const std::string& name ) const;
    void readMdh( std::istream& is, Mdh& mdh ) const;
    Volume< uint32_t > getFidOffsets( std::istream& is,
                                      const HeaderedObject& object ) const;

};


}


#endif
