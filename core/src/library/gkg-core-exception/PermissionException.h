#ifndef _gkg_core_exception_PermissionException_h_
#define _gkg_core_exception_PermissionException_h_


#include <gkg-core-exception/OpenException.h>


namespace gkg
{


class PermissionException : public OpenException
{

  public:

    PermissionException( const std::string& argument,
                           const std::string& fileName );
    PermissionException( const std::string& fileName = "" );
    virtual ~PermissionException() throw();

};


}


#endif

