#ifndef _gkg_core_exception_FormatMismatchException_h_
#define _gkg_core_exception_FormatMismatchException_h_


#include <gkg-core-exception/FormatException.h>


namespace gkg
{


class FormatMismatchException : public FormatException
{

  public:

    FormatMismatchException( const std::string& argument,
                             const std::string& fileName );
    FormatMismatchException( const std::string& fileName = "" );
    virtual ~FormatMismatchException() throw();

};


}


#endif

