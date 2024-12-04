#ifndef _gkg_processing_plugin_nifti_NiftiDiskFormat_h_
#define _gkg_processing_plugin_nifti_NiftiDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject.h>

#include <pio/nifti/nifti2/nifti2_io.h>


namespace gkg
{


template < class T > class Volume;
class RegionInformation;


template < class T >
class NiftiDiskFormat : public TypedDiskFormat< Volume< T > >,
                        public Singleton< NiftiDiskFormat< T > >
{

  public:

    ~NiftiDiskFormat();

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

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< NiftiDiskFormat< T > >;

    NiftiDiskFormat();

    void getInt32Value( const std::string& name,
                        const HeaderedObject& other,
                        int32_t& value,
                        int32_t defaultValue = 0 ) const;
    void getInt64Value( const std::string& name,
                        const HeaderedObject& other,
                        int64_t& value,
                        int64_t defaultValue = 0 ) const;
    void getFloatValue( const std::string& name,
                        const HeaderedObject& other,
                        float& value,
                        float defaultValue = 0.0 ) const;
    void getDoubleValue( const std::string& name,
                         const HeaderedObject& other,
                         double& value,
                         double defaultValue = 0.0 ) const;
    void getStringValue( const std::string& name,
                         const HeaderedObject& other,
                         std::string& value,
                         const std::string& defaultValue = "" ) const;

};


}


#endif
