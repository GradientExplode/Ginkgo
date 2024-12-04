#ifndef _gkg_core_exception_MissingOptionException_h_
#define _gkg_core_exception_MissingOptionException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class MissingOptionException : public std::runtime_error
{

  public:

    MissingOptionException( const std::string& optionName );
    virtual ~MissingOptionException() throw();

};


}


#endif
