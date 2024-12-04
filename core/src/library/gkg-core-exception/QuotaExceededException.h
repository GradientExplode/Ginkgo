#ifndef _gkg_core_exception_QuotaExceededException_h_
#define _gkg_core_exception_QuotaExceededException_h_


#include <gkg-core-exception/DiskFullException.h>


namespace gkg
{


class QuotaExceededException : public DiskFullException
{

  public:

    QuotaExceededException( const std::string& argument,
                            const std::string& fileName );
    QuotaExceededException( const std::string& fileName = "" );
    virtual ~QuotaExceededException() throw();

};


}


#endif

