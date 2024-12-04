#ifndef _gkg_core_exception_TooManyDescriptorsException_h_
#define _gkg_core_exception_TooManyDescriptorsException_h_


#include <gkg-core-exception/OpenException.h>


namespace gkg
{


class TooManyDescriptorsException : public OpenException
{

  public:

    TooManyDescriptorsException( const std::string& argument,
                                 const std::string& fileName );
    TooManyDescriptorsException( const std::string& fileName = "" );
    virtual ~TooManyDescriptorsException() throw();

};


}


#endif

