#ifndef _gkg_core_exception_NonReversibleMatrixException_h_
#define _gkg_core_exception_NonReversibleMatrixException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class NonReversibleMatrixException : public std::runtime_error
{

  public:

    NonReversibleMatrixException( const std::string& argument = "" );
    virtual ~NonReversibleMatrixException() throw();

};


}


#endif
