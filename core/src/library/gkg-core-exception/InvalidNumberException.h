#ifndef _gkg_core_exception_InvalidNumberException_h_
#define _gkg_core_exception_InvalidNumberException_h_


#include <gkg-core-exception/FormatException.h>


namespace gkg
{


class InvalidNumberException : public FormatException
{

  public:

    InvalidNumberException( const std::string& value,
                            const std::string& name = "number" );
    virtual ~InvalidNumberException() throw();

};


}


#endif

