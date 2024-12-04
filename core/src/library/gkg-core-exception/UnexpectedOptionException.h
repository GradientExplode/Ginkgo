#ifndef _gkg_core_exception_UnexpectedOptionException_h_
#define _gkg_core_exception_UnexpectedOptionException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class UnexpectedOptionException : public std::runtime_error
{

  public:

    UnexpectedOptionException( const std::string& optionName );
    virtual ~UnexpectedOptionException() throw();

};


}


#endif
