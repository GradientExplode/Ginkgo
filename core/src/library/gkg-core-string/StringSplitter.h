#ifndef _gkg_core_string_StringSplitter_h_
#define _gkg_core_string_StringSplitter_h_


#include <vector>
#include <string>


namespace gkg
{


class StringSplitter
{

  public:

    typedef std::vector< std::string >::const_iterator const_iterator;

    StringSplitter( std::string& strIn, const std::string& delimiters = "\n" );
    virtual ~StringSplitter();

    const_iterator begin() const;
    const_iterator end() const;

    std::vector< std::string > getTokens();

  private:

    void split( std::string& strIn, const std::string& delimiters );

    std::vector< std::string > _tokens;

};


}



#endif
