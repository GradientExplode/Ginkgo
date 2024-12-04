#include <gkg-core-exception/ParseException.h>
#include <gkg-core-cppext/StringStream.h>


gkg::ParseException::ParseException( const std::string& what,
                                     const std::string& token,
                                     const std::string& fileName,
                                     int32_t line )
                    : gkg::FormatException( initialize( what,
                                                        token,
                                                        fileName,
                                                        line ), "" ),
                      _fileName( fileName ),
                      _line( line )
{
}


gkg::ParseException::ParseException( const std::string& what,
                                     const std::string& token,
                                     int32_t line )
                    : gkg::FormatException( initialize( what,
                                                        token,
                                                        line ), "" ),
                      _line( line )
{
}



gkg::ParseException::~ParseException() throw()
{
}


int32_t gkg::ParseException::getLine() const
{

  return _line;

}


std::string gkg::ParseException::initialize( const std::string& what,
                                             const std::string& token,
                                             const std::string& fileName,
                                             int32_t line )
{

  std::string s( fileName );
  if ( !fileName.empty() )
  {

    s += ": ";

  }
  s += "found \"";
  s += token;
  s += "\" while waiting for \"";
  s += what;
  s += "\"";
  if ( line )
  {

    std::stringstream ss;
    ss << " (line " << line << ")";
    s += ss.str();

  }
  return s;

}


std::string gkg::ParseException::initialize( const std::string& what,
                                             const std::string& token,
                                             int32_t line )
{

  std::string s( "found \"" );
  s += token;
  s += "\" while waiting for \"";
  s += what;
  s += "\"";
  if ( line )
  {

    std::stringstream ss;
    ss << " (line " << line << ")";
    s += ss.str();

  }
  return s;

}

