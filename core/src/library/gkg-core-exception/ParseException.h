#ifndef _gkg_core_exception_ParseException_h_
#define _gkg_core_exception_ParseException_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/FormatException.h>


namespace gkg
{


class ParseException : public FormatException
{

  public:

    ParseException( const std::string& what,
                    const std::string& token,
                    const std::string& fileName,
                    int32_t line );
    ParseException( const std::string& what,
                    const std::string& token,
                    int32_t line );
    virtual ~ParseException() throw();

    int32_t getLine() const;

  private:

    static std::string initialize( const std::string& what,
                                   const std::string& token,
                                   const std::string& fileName,
                                   int32_t line );

    static std::string initialize( const std::string& what,
                                   const std::string& token,
                                   int32_t line );

    const std::string _fileName;
    const int32_t _line;

};


}


#endif

