#include <gkg-core-exception/SyntaxCheckException.h>


gkg::SyntaxCheckException::SyntaxCheckException( const std::string& argument,
                                                 const std::string& fileName )
                          : gkg::InvalidFormatException( argument, fileName )
{
}


gkg::SyntaxCheckException::SyntaxCheckException( const std::string& fileName )
                          : gkg::InvalidFormatException( "syntax check error",
                                                         fileName )
{
}


gkg::SyntaxCheckException::~SyntaxCheckException() throw()
{
}


