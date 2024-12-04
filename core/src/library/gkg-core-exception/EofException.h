#ifndef _gkg_core_exception_EofException_h_
#define _gkg_core_exception_EofException_h_


#include <gkg-core-exception/StreamStateException.h>


namespace gkg
{


class EofException : public StreamStateException
{

  public:

    EofException( const std::string& argument,
                  const std::string& fileName );
    EofException( const std::string& fileName = "" );
    virtual ~EofException() throw();

};


}


#endif

