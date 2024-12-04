#ifndef _gkg_core_exception_ErrnoException_h_
#define _gkg_core_exception_ErrnoException_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


class ErrnoException : public std::runtime_error
{

  public:

    ErrnoException();
    virtual ~ErrnoException() throw ();

    int32_t getThrownErrno() const;

  protected:

    ErrnoException( const std::string& what );

  private:

    static std::string initialize();
    const int32_t _gkgErrno;

};


}


#endif
