#ifndef _gkg_core_io_DiskFormat_h_
#define _gkg_core_io_DiskFormat_h_


#include <gkg-core-object/HeaderedObject.h>
#include <string>
#include <set>


namespace gkg
{


class DiskFormat
{

  public:

    virtual ~DiskFormat();

    virtual std::string getName() const = 0;

    virtual bool hasReader() const;
    virtual bool hasWriter() const;

    virtual std::string getNameWithoutExtension( const std::string& name) const;
    virtual bool hasMatchingExtension( const std::string& name ) const;
    virtual std::string getHeaderExtension() const;

    virtual void readHeader( const std::string& name,
                             HeaderedObject& object ) const;
    virtual void writeHeader( const std::string& name,
                              const HeaderedObject& object ) const;

  protected:

    DiskFormat();
    std::set< std::string > _extensions;

};


}


#endif
