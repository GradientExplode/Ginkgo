#ifndef _gkg_core_exception_IsDirException_h_
#define _gkg_core_exception_IsDirException_h_


#include <gkg-core-exception/OpenException.h>


namespace gkg
{


class IsDirException : public OpenException
{

  public:

    IsDirException( const std::string& argument,
                           const std::string& fileName );
    IsDirException( const std::string& fileName = "" );
    virtual ~IsDirException() throw();

};


}


#endif

