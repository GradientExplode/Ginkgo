#ifndef _gkg_simulation_io_MedusaGeometryDiskFormat_h_
#define _gkg_simulation_io_MedusaGeometryDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class VirtualTissue;


class MedusaGeometryDiskFormat : public TypedDiskFormat< VirtualTissue >,
                                 public Singleton< MedusaGeometryDiskFormat >
{

  public:

    ~MedusaGeometryDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;
    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               VirtualTissue& object ) const;
    void write( const std::string& name,
                VirtualTissue& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< MedusaGeometryDiskFormat >;

    MedusaGeometryDiskFormat();

};


}


#endif

