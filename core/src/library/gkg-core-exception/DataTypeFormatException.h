#ifndef _gkg_core_exception_DataTypeFormatException_h_
#define _gkg_core_exception_DataTypeFormatException_h_


#include <gkg-core-exception/InvalidFormatException.h>


namespace gkg
{


class DataTypeFormatException : public InvalidFormatException
{

  public:

    DataTypeFormatException( const std::string& argument,
                             const std::string& fileName );
    DataTypeFormatException( const std::string& fileName = "" );
    virtual ~DataTypeFormatException() throw();

};


}


#endif

