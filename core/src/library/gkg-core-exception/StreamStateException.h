#ifndef _gkg_core_exception_StreamStateException_h_
#define _gkg_core_exception_StreamStateException_h_


#include <gkg-core-exception/ReadWriteException.h>


namespace gkg
{


class StreamStateException : public ReadWriteException
{

  public:

    StreamStateException( const std::string& argument,
                           const std::string& fileName = "" );
    virtual ~StreamStateException() throw();

};


}


#endif

