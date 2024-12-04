#include <gkg-core-exception/WrongFormatException.h>


gkg::WrongFormatException::WrongFormatException( const std::string& argument,
                                                 const std::string& fileName )
                          : gkg::FormatException( argument, fileName )
{
}


gkg::WrongFormatException::WrongFormatException( const std::string& fileName )
                          : gkg::FormatException( "unrecognized format",
                                                  fileName )
{
}


gkg::WrongFormatException::~WrongFormatException() throw()
{
}


