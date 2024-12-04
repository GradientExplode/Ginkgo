#include <gkg-core-exception/TooManyDescriptorsException.h>


gkg::TooManyDescriptorsException::TooManyDescriptorsException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                                 : gkg::OpenException( argument, fileName )
{
}


gkg::TooManyDescriptorsException::TooManyDescriptorsException( 
                                                   const std::string& fileName )
                                 : gkg::OpenException( 
                                         "too many open descriptors", fileName )
{
}


gkg::TooManyDescriptorsException::~TooManyDescriptorsException() throw()
{
}


