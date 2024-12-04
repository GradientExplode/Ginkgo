#include <gkg-core-exception/CorruptStreamException.h>


gkg::CorruptStreamException::CorruptStreamException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                            : gkg::FormatMismatchException( argument, fileName )
{
}


gkg::CorruptStreamException::CorruptStreamException(
                                                   const std::string& fileName )
                            : gkg::FormatMismatchException( "corrupt stream",
                                                            fileName )
{
}


gkg::CorruptStreamException::~CorruptStreamException() throw()
{
}


