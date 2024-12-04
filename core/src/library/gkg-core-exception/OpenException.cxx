#include <gkg-core-exception/OpenException.h>


gkg::OpenException::OpenException( const std::string& argument,
                                   const std::string& fileName )
                   : gkg::IOException( argument, fileName )
{
}


gkg::OpenException::~OpenException() throw()
{
}


