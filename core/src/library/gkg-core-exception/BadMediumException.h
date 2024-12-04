#ifndef _gkg_core_exception_BadMediumException_h_
#define _gkg_core_exception_BadMediumException_h_


#include <gkg-core-exception/ReadWriteException.h>


namespace gkg
{


class BadMediumException : public ReadWriteException
{

  public:

    BadMediumException( const std::string& argument,
                           const std::string& fileName );
    BadMediumException( const std::string& fileName = "" );
    virtual ~BadMediumException() throw();

};


}


#endif

