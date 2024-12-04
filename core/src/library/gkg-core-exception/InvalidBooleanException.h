#ifndef _gkg_core_exception_InvalidBooleanException_h_
#define _gkg_core_exception_InvalidBooleanException_h_


#include <gkg-core-exception/FormatException.h>


namespace gkg
{


class InvalidBooleanException : public FormatException
{

  public:

    InvalidBooleanException( const std::string& value );
    virtual ~InvalidBooleanException() throw();

};


}


#endif

