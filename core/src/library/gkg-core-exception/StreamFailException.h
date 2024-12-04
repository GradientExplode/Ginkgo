#ifndef _gkg_core_exception_StreamFailException_h_
#define _gkg_core_exception_StreamFailException_h_


#include <gkg-core-exception/StreamStateException.h>


namespace gkg
{


class StreamFailException : public StreamStateException
{

  public:

    StreamFailException( const std::string& argument,
                         const std::string& fileName );
    StreamFailException( const std::string& fileName = "" );
    virtual ~StreamFailException() throw();

};


}


#endif

