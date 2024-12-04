#ifndef _gkg_core_exception_ObjectTypeException_h_
#define _gkg_core_exception_ObjectTypeException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class ObjectTypeException : public IOException
{

  public:

    ObjectTypeException( const std::string& argument,
                         const std::string& fileName );
    ObjectTypeException( const std::string& fileName = "" );
    virtual ~ObjectTypeException() throw();

};


}


#endif

