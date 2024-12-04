#ifndef _gkg_core_exception_FileException_h_
#define _gkg_core_exception_FileException_h_


#include <gkg-core-exception/ErrnoException.h>


namespace gkg
{


class FileException : public ErrnoException
{

  public:

    FileException( const std::string& fileName );
    FileException( const std::string& what, const std::string& fileName );
    virtual ~FileException() throw ();

    const std::string& getFileName() const;

  private:

    static std::string initialize( const std::string& filename );

    const std::string _fileName;

};


}


#endif
