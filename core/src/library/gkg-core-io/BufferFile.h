#ifndef _gkg_core_io_BufferFile_h_
#define _gkg_core_io_BufferFile_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>


namespace gkg
{


struct BufferFileInfo 
{

  BufferFileInfo( const std::string& theName, int32_t theFd );

  std::string name;
  int32_t fd;
  struct stat info;
  off_t position;
  char* buffer;
  uint32_t limit;

};


class BufferFile
{

  public:

    BufferFile();
    virtual ~BufferFile();

    virtual void open( const std::string& theName ) = 0;
    virtual const std::string& getName() const;
    virtual int64_t getLength() const;
    virtual void close();

    virtual void setLimit( uint32_t bufferSize );

    operator bool();

  private:

    void operator=( const BufferFile& );  // not allowed

  protected:

    BufferFileInfo* _bufferFileInfo;


};



}


#endif
