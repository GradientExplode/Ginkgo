#ifndef _gkg_core_exception_IOException_h_
#define _gkg_core_exception_IOException_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


class IOException : public std::runtime_error
{

  public:

    IOException( const std::string& argument,
                 const std::string& fileName = "" );
    virtual ~IOException() throw();

    virtual const char* what() const throw();

    const std::string& getFileName() const;

    static void launchErrno( const std::string& fileName = "" );
    static void keepPriority( std::exception& e,
                              int32_t& priority, 
                              int32_t& type,
                              std::string& message, 
                              int32_t raisePriority = 0 );
    static void launch( int32_t code,
                        const std::string& message, 
                        const std::string& defaultMessage = "" );

  private:

    std::string _fileName;
    mutable std::string _formedMessage;

};


}


#endif
