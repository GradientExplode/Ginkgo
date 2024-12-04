#ifndef _gkg_fmri_io_ParadigmDiskFormat_h_
#define _gkg_fmri_io_ParadigmDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class Paradigm;


class ParadigmDiskFormat : public TypedDiskFormat< Paradigm >,
                           public Singleton< ParadigmDiskFormat >
{

  public:

    ~ParadigmDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               Paradigm& object ) const;
    void write( const std::string& name,
                Paradigm& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< ParadigmDiskFormat >;

    ParadigmDiskFormat();

};


}


#endif
