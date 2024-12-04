#include <gkg-core-exception/QuotaExceededException.h>


gkg::QuotaExceededException::QuotaExceededException( const std::string& argument,
                                                     const std::string& fileName )
                            : gkg::DiskFullException( argument, fileName )
{
}


gkg::QuotaExceededException::QuotaExceededException( const std::string& fileName )
                            : gkg::DiskFullException( "disk quota exceeded",
                                                      fileName )
{
}


gkg::QuotaExceededException::~QuotaExceededException() throw()
{
}


