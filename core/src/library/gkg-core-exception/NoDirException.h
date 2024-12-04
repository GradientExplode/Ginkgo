#ifndef _gkg_core_exception_NoDirException_h_
#define _gkg_core_exception_NoDirException_h_


#include <gkg-core-exception/FileNotFoundException.h>


namespace gkg
{


class NoDirException : public FileNotFoundException
{

  public:

    NoDirException( const std::string& argument,
                           const std::string& fileName );
    NoDirException( const std::string& fileName = "" );
    virtual ~NoDirException() throw();

};


}


#endif

