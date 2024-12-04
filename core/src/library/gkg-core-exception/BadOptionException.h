#ifndef _gkg_core_exception_BadOptionException_h_
#define _gkg_core_exception_BadOptionException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class BadOptionException : public std::logic_error
{

  public:

    BadOptionException( const std::string& name );
    virtual ~BadOptionException() throw();

};


}


#endif
