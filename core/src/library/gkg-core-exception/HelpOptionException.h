#ifndef _gkg_core_exception_HelpOptionException_h_
#define _gkg_core_exception_HelpOptionException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class HelpOptionException : public std::runtime_error
{

  public:

    HelpOptionException();
    virtual ~HelpOptionException() throw();

};


}


#endif
