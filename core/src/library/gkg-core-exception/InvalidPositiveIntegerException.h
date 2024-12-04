#ifndef _gkg_core_exception_InvalidPositiveIntegerException_h_
#define _gkg_core_exception_InvalidPositiveIntegerException_h_


#include <gkg-core-exception/InvalidIntegerException.h>


namespace gkg
{


class InvalidPositiveIntegerException : public InvalidIntegerException
{

  public:

    InvalidPositiveIntegerException( const std::string& value,
                             const std::string& name = "positive integer" );
    virtual ~InvalidPositiveIntegerException() throw();

};


}


#endif

