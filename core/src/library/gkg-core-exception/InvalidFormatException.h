#ifndef _gkg_core_exception_InvalidFormatException_h_
#define _gkg_core_exception_InvalidFormatException_h_


#include <gkg-core-exception/FormatMismatchException.h>


namespace gkg
{


class InvalidFormatException : public FormatMismatchException
{

  public:

    InvalidFormatException( const std::string& argument,
                            const std::string& fileName );
    InvalidFormatException( const std::string& fileName = "" );
    virtual ~InvalidFormatException() throw();

};


}


#endif

