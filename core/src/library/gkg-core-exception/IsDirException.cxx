#include <gkg-core-exception/IsDirException.h>


gkg::IsDirException::IsDirException( const std::string& argument,
                                     const std::string& fileName )
                    : gkg::OpenException( argument, fileName )
{
}


gkg::IsDirException::IsDirException( const std::string& fileName )
                    : gkg::OpenException(
                           "attempt to access a directory as a file", fileName )
{
}


gkg::IsDirException::~IsDirException() throw()
{
}


