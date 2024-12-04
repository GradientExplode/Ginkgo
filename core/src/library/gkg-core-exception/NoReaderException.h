#ifndef _gkg_core_exception_NoReaderException_h_
#define _gkg_core_exception_NoReaderException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class NoReaderException : public IOException
{

  public:

    NoReaderException( const std::string& argument,
                       const std::string& fileName );
    NoReaderException( const std::string& fileName = "" );
    virtual ~NoReaderException() throw();

};


}


#endif

