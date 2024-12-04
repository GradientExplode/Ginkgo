#include <gkg-core-exception/BadMediumException.h>


gkg::BadMediumException::BadMediumException( const std::string& argument,
                                             const std::string& fileName )
                        : gkg::ReadWriteException( argument, fileName )
{
}


gkg::BadMediumException::BadMediumException( const std::string& fileName )
                        : gkg::ReadWriteException(
                              "bad medium (disk or network failure)", fileName )
{
}


gkg::BadMediumException::~BadMediumException() throw()
{
}


