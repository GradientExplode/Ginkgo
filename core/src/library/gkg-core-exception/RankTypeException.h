#ifndef _gkg_core_exception_RankTypeException_h_
#define _gkg_core_exception_RankTypeException_h_


#include <gkg-core-exception/IOException.h>


namespace gkg
{


class RankTypeException : public IOException
{

  public:

    RankTypeException( const std::string& argument,
                       const std::string& fileName );
    RankTypeException( const std::string& fileName = "" );
    virtual ~RankTypeException() throw();

};


}


#endif

