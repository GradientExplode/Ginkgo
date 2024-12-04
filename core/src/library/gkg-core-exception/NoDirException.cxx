#include <gkg-core-exception/NoDirException.h>


gkg::NoDirException::NoDirException( const std::string& argument,
                                     const std::string& fileName )
                    : gkg::FileNotFoundException( argument, fileName )
{
}


gkg::NoDirException::NoDirException( const std::string& fileName )
                    : gkg::FileNotFoundException( "invalid directory",
                                                  fileName )
{
}


gkg::NoDirException::~NoDirException() throw()
{
}


