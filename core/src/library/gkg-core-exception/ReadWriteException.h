#ifndef _gkg_core_exception_ReadWriteException_h_
#define _gkg_core_exception_ReadWriteException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class ReadWriteException : public IOException
{

  public:

    ReadWriteException( const std::string& argument,
                        const std::string& fileName = "" );
    virtual ~ReadWriteException() throw();

};


}


#endif

