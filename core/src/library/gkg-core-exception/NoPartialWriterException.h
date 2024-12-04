#ifndef _gkg_core_exception_NoPartialWriterException_h_
#define _gkg_core_exception_NoPartialWriterException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class NoPartialWriterException : public IOException
{

  public:

    NoPartialWriterException( const std::string& argument,
                              const std::string& fileName );
    NoPartialWriterException( const std::string& fileName = "" );
    virtual ~NoPartialWriterException() throw();

};


}


#endif

