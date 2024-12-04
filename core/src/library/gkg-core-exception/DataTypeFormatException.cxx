#include <gkg-core-exception/DataTypeFormatException.h>


gkg::DataTypeFormatException::DataTypeFormatException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                             : gkg::InvalidFormatException( argument, fileName )
{
}


gkg::DataTypeFormatException::DataTypeFormatException(
                                                   const std::string& fileName )
                             : gkg::InvalidFormatException(
                                             "unsupported data type", fileName )
{
}


gkg::DataTypeFormatException::~DataTypeFormatException() throw()
{
}


