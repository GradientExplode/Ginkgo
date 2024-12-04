#ifndef _gkg_core_exception_DiskFullException_h_
#define _gkg_core_exception_DiskFullException_h_


#include <gkg-core-exception/ReadWriteException.h>


namespace gkg
{


class DiskFullException : public ReadWriteException
{

  public:

    DiskFullException( const std::string& argument,
                       const std::string& fileName );
    DiskFullException( const std::string& fileName = "" );
    virtual ~DiskFullException() throw();

};


}


#endif

