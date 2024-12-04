#ifndef _gkg_core_exception_SyntaxCheckException_h_
#define _gkg_core_exception_SyntaxCheckException_h_


#include <gkg-core-exception/InvalidFormatException.h>


namespace gkg
{


class SyntaxCheckException : public InvalidFormatException
{

  public:

    SyntaxCheckException( const std::string& argument,
                          const std::string& fileName );
    SyntaxCheckException( const std::string& fileName = "" );
    virtual ~SyntaxCheckException() throw();

};


}


#endif

