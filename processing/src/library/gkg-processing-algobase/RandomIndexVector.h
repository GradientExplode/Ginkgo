#ifndef _gkg_processing_algobase_RandomIndexVector_h_
#define _gkg_processing_algobase_RandomIndexVector_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T >
void getRandomIndexVector( std::vector< T >& data );


template < class T >
void getRandomIndexVector( std::vector< T >& data, const T& maximumIndex );


void getRandomSelection( std::vector< bool >& selection,
                         int32_t selectedCount );


}


#endif
