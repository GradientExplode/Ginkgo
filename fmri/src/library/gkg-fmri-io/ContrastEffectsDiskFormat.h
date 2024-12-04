#ifndef _gkg_fmri_io_ContrastEffectsDiskFormat_h_
#define _gkg_fmri_io_ContrastEffectsDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ContrastEffects;


class ContrastEffectsDiskFormat : public TypedDiskFormat< ContrastEffects >,
                                  public Singleton< ContrastEffectsDiskFormat >
{

  public:

    ~ContrastEffectsDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               ContrastEffects& object ) const;
    void write( const std::string& name,
                ContrastEffects& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< ContrastEffectsDiskFormat >;

    ContrastEffectsDiskFormat();

};


}


#endif
