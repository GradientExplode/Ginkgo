#ifndef _gkg_core_exception_FileNotFoundException_h_
#define _gkg_core_exception_FileNotFoundException_h_


#include <gkg-core-exception/OpenException.h>


namespace gkg
{


class FileNotFoundException : public OpenException
{

  public:

    FileNotFoundException( const std::string& argument,
                           const std::string& fileName );
    FileNotFoundException( const std::string& fileName = "" );
    virtual ~FileNotFoundException() throw();

};


}


#endif

