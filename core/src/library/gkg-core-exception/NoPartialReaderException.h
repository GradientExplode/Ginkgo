#ifndef _gkg_core_exception_NoPartialReaderException_h_
#define _gkg_core_exception_NoPartialReaderException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class NoPartialReaderException : public IOException
{

  public:

    NoPartialReaderException( const std::string& argument,
                              const std::string& fileName );
    NoPartialReaderException( const std::string& fileName = "" );
    virtual ~NoPartialReaderException() throw();

};


}


#endif

