#ifndef _gkg_core_io_InputBufferFile_h_
#define _gkg_core_io_InputBufferFile_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-io/BufferFile.h>


namespace gkg
{


class InputBufferFile : public BufferFile
{

  public:

    InputBufferFile();
    InputBufferFile( const std::string& theName );
    virtual ~InputBufferFile();

    virtual void open( const std::string& theName );
    virtual int32_t read( const char*& start );

};


}


#endif
