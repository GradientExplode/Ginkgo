#ifndef _gkg_processing_plugin_dicom_DicomDiskFormat_h_
#define _gkg_processing_plugin_dicom_DicomDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <vector>


namespace gkg
{


template < class T > class Volume;
class RegionInformation;


template < class T >
class DicomDiskFormat : public TypedDiskFormat< Volume< T > >,
                        public Singleton< DicomDiskFormat< T > >
{

  public:

    ~DicomDiskFormat();

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

  protected:

    friend class Singleton< DicomDiskFormat< T > >;

    DicomDiskFormat();

};


}


#endif
