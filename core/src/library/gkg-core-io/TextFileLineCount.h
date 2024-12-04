#ifndef _gkg_core_io_TextFileLineCount_h_
#define _gkg_core_io_TextFileLineCount_h_


#include <string>
#include <vector>


namespace gkg
{


int32_t getTextFileLineCount( const std::string& fileName );

std::vector< int32_t > getScalarItemCountPerLine( const std::string& fileName );

std::vector< int32_t > getStringItemCountPerLine( const std::string& fileName );


}


#endif
