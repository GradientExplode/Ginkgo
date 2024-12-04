#ifndef _gkg_core_exception_InconsistentHeaderException_h_
#define _gkg_core_exception_InconsistentHeaderException_h_


#include <gkg-core-exception/Exception.h>


namespace gkg
{


class InconsistentHeaderException : public std::runtime_error
{

  public:

    InconsistentHeaderException( const std::string& argument,
                                 const std::string& fileName = "" );
    virtual ~InconsistentHeaderException() throw();

    virtual const char* what() const throw();

    const std::string& getFileName() const;

  private:

    std::string _fileName;
    mutable std::string _formedMessage;

};


}


#endif
