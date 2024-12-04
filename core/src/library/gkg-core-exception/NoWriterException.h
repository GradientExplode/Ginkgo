#ifndef _gkg_core_exception_NoWriterException_h_
#define _gkg_core_exception_NoWriterException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class NoWriterException : public IOException
{

  public:

    NoWriterException( const std::string& argument,
                       const std::string& fileName );
    NoWriterException( const std::string& fileName = "" );
    virtual ~NoWriterException() throw();

};


}


#endif

