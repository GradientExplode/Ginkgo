#ifndef _gkg_core_exception_CorruptStreamException_h_
#define _gkg_core_exception_CorruptStreamException_h_


#include <gkg-core-exception/FormatMismatchException.h>


namespace gkg
{


class CorruptStreamException : public FormatMismatchException
{

  public:

    CorruptStreamException( const std::string& argument,
                            const std::string& fileName );
    CorruptStreamException( const std::string& fileName = "" );
    virtual ~CorruptStreamException() throw();

};


}


#endif

