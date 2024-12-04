#ifndef _gkg_core_io_TypedDiskFormat_h_
#define _gkg_core_io_TypedDiskFormat_h_


#include <gkg-core-io/DiskFormat.h>


namespace gkg
{


class RegionInformation;


template < class O >
class TypedDiskFormat : public DiskFormat
{

  public:

    virtual ~TypedDiskFormat();

    virtual void read( const std::string& name,
                       O& object ) const;
    virtual void write( const std::string& name,
                        O& object,
                        bool ascii ) const;

    virtual void preparePartialBinaryRead( const std::string& name,
                                           HeaderedObject& proxy ) const;
    virtual void partialBinaryRead(
                             const HeaderedObject& proxy,
                             GenericObject& subpart,
                             const RegionInformation& regionInformation ) const;

    virtual void preparePartialBinaryWrite( const std::string& name,
                                            HeaderedObject& proxy ) const;
    virtual void partialBinaryWrite(
                             const HeaderedObject& proxy,
                             const GenericObject& subpart,
                             const RegionInformation& regionInformation ) const;

  protected:

    TypedDiskFormat();

};


}


#endif
