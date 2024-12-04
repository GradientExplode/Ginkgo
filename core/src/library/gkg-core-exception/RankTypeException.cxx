#include <gkg-core-exception/RankTypeException.h>


gkg::RankTypeException::RankTypeException( const std::string& argument,
                                           const std::string& fileName )
                       : gkg::IOException( argument, fileName )
{
}


gkg::RankTypeException::RankTypeException( const std::string& fileName )
                       : gkg::IOException( "rank type error", fileName )
{
}


gkg::RankTypeException::~RankTypeException() throw()
{
}


