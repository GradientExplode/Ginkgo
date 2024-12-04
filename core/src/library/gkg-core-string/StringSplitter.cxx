#include <gkg-core-string/StringSplitter.h>


gkg::StringSplitter::StringSplitter( std::string& strIn,
                                     const std::string& delimiters )
{

  this->split( strIn, delimiters );

}


gkg::StringSplitter::~StringSplitter()
{
}


gkg::StringSplitter::const_iterator gkg::StringSplitter::begin() const
{

  return _tokens.begin();

}


gkg::StringSplitter::const_iterator gkg::StringSplitter::end() const
{

  return _tokens.end();

}


std::vector< std::string > gkg::StringSplitter::getTokens()
{

  return _tokens;

}


void gkg::StringSplitter::split( std::string& strIn, 
                                 const std::string& delimiters )
{

  std::string::size_type i = 0, len = strIn.length();

  while ( i < len )
  {

    i = strIn.find_first_not_of( delimiters, i );

    if ( i == std::string::npos )
    {

      return;

    }

    std::string::size_type j = strIn.find_first_of( delimiters, i );

    if ( j == std::string::npos )
    {

      _tokens.push_back( strIn.substr( i ) );
      return;

    }
    else
    {

      _tokens.push_back( strIn.substr( i, j - i ) );

    }

    i = j + 1;

  }

}
