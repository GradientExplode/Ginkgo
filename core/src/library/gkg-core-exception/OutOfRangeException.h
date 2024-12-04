#ifndef _gkg_core_exception_OutOfRangeException_h_
#define _gkg_core_exception_OutOfRangeException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class OutOfRangeException : public std::range_error
{

  public:

    OutOfRangeException( const std::string& value );
    virtual ~OutOfRangeException() throw();

};


}


#endif

