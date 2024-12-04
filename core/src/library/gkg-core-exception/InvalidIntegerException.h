#ifndef _gkg_core_exception_InvalidIntegerException_h_
#define _gkg_core_exception_InvalidIntegerException_h_


#include <gkg-core-exception/InvalidNumberException.h>


namespace gkg
{


class InvalidIntegerException : public InvalidNumberException
{

  public:

    InvalidIntegerException( const std::string& value,
                             const std::string& name = "integer" );
    virtual ~InvalidIntegerException() throw();

};


}


#endif

