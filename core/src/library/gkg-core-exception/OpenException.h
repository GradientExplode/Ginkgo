#ifndef _gkg_core_exception_OpenException_h_
#define _gkg_core_exception_OpenException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class OpenException : public IOException
{

  public:

    OpenException( const std::string& argument,
                   const std::string& fileName = "" );
    virtual ~OpenException() throw();

};


}


#endif

