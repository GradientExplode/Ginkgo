#ifndef _gkg_core_exception_WrongFormatException_h_
#define _gkg_core_exception_WrongFormatException_h_


#include <gkg-core-exception/FormatException.h>


namespace gkg
{


class WrongFormatException : public FormatException
{

  public:

    WrongFormatException( const std::string& argument,
                          const std::string& fileName );
    WrongFormatException( const std::string& fileName = "" );
    virtual ~WrongFormatException() throw();

};


}


#endif

