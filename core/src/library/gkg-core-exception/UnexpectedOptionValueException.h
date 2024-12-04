#ifndef _gkg_core_exception_UnexpectedOptionValueException_h_
#define _gkg_core_exception_UnexpectedOptionValueException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class UnexpectedOptionValueException : public std::runtime_error
{

  public:

    UnexpectedOptionValueException( const std::string& optionName,
                                    const std::string& value );
    virtual ~UnexpectedOptionValueException() throw();

};


}


#endif
