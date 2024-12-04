#ifndef _gkg_processing_io_LabelledRoiDictionaryDiskFormat_h_
#define _gkg_processing_io_LabelledRoiDictionaryDiskFormat_h_

#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class LabelledRoiDictionary;


class LabelledRoiDictionaryDiskFormat : 
                             public TypedDiskFormat< LabelledRoiDictionary >,
                             public Singleton< LabelledRoiDictionaryDiskFormat >
{

  public:
  
    ~LabelledRoiDictionaryDiskFormat();
    
    std::string getName() const;
    
    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void read( const std::string& name,
               LabelledRoiDictionary& object ) const;

    void write( const std::string& name,
                LabelledRoiDictionary& object,
                bool ascii ) const;	       

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< LabelledRoiDictionaryDiskFormat >;

    LabelledRoiDictionaryDiskFormat();

};


}


#endif
    
    
