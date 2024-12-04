#ifndef _gkg_core_exception_NotOpenException_h_
#define _gkg_core_exception_NotOpenException_h_


#include <gkg-core-exception/StreamStateException.h>


namespace gkg
{


class NotOpenException : public StreamStateException
{

  public:

    NotOpenException( const std::string& argument,
                      const std::string& fileName );
    NotOpenException( const std::string& fileName = "" );
    virtual ~NotOpenException() throw();

};


}


#endif

