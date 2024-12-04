#include <gkg-core-exception/FileNotFoundException.h>


gkg::FileNotFoundException::FileNotFoundException( const std::string& argument,
                                                   const std::string& fileName )
                           : gkg::OpenException( argument, fileName )
{
}


gkg::FileNotFoundException::FileNotFoundException( const std::string& fileName )
                           : gkg::OpenException( "file not found", fileName )
{
}


gkg::FileNotFoundException::~FileNotFoundException() throw()
{
}


