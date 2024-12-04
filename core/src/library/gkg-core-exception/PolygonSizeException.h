#ifndef _gkg_core_exception_PolygonSizeException_h_
#define _gkg_core_exception_PolygonSizeException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class PolygonSizeException : public IOException
{

  public:

    PolygonSizeException( const std::string& argument,
                          const std::string& fileName );
    PolygonSizeException( const std::string& fileName = "" );
    virtual ~PolygonSizeException() throw();

};


}


#endif

