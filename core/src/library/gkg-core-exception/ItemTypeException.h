#ifndef _gkg_core_exception_ItemTypeException_h_
#define _gkg_core_exception_ItemTypeException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class ItemTypeException : public IOException
{

  public:

    ItemTypeException( const std::string& argument,
                       const std::string& fileName );
    ItemTypeException( const std::string& fileName = "" );
    virtual ~ItemTypeException() throw();

};


}


#endif

