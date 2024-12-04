#include <gkg-core-exception/OutOfRangeException.h>


gkg::OutOfRangeException::OutOfRangeException( const std::string& value )
                         : std::range_error( value + " is out if range" )
{
}


gkg::OutOfRangeException::~OutOfRangeException() throw()
{
}


