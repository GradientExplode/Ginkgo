#ifndef _gkg_core_exception_BadParameterCountException_h_
#define _gkg_core_exception_BadParameterCountException_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


class BadParameterCountException : public std::runtime_error
{

  public:

    BadParameterCountException( int32_t count,
                                const std::string& argument = "" );
    virtual ~BadParameterCountException() throw();

};


}


#endif
