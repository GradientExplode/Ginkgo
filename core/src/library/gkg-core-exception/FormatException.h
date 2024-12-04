#ifndef _gkg_core_exception_FormatException_h_
#define _gkg_core_exception_FormatException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class FormatException : public IOException
{

  public:

    FormatException( const std::string& argument,
                     const std::string& fileName );
    FormatException( const std::string& fileName = "" );
    virtual ~FormatException() throw();

};


}


#endif

