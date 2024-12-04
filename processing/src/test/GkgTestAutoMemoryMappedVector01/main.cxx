#include <gkg-processing-container/AutoMemoryMappedVector_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <vector>

#define MEM_SIZE ( uint64_t )100 * ( uint64_t )1024 * ( uint64_t )1024
  
#define COUNT 10000


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::vector< gkg::AutoMemoryMappedVector< int8_t >* >
      vectors( COUNT );

    int32_t i = 0;
    for ( i = 0; i < COUNT; i++ )
    {

       vectors[ i ] = new gkg::AutoMemoryMappedVector< int8_t >( MEM_SIZE );
       std::cout << "memory mapped [" << i << "] : "
                << vectors[ i ]->isMemoryMapped()
                << std::endl;

       if ( vectors[ i ]->isMemoryMapped() )
       {

         // testing the operator[]
         if ( i % 2 )
         {

           uint64_t j = 0;
           for ( j = 0; j < MEM_SIZE; j++ )
           {

             ( *vectors[ i ] )[ j ] = j % 127;

             std::cout << j << " -> " << ( int32_t )( *vectors[ i ] )[ j ]
                       << std::endl;

           }

         }
         else
         {

           uint64_t count = 0;
           gkg::AutoMemoryMappedVector< int8_t >::iterator
             j = vectors[ i ]->begin(),
             je = vectors[ i ]->end();
           while ( j != je )
           {

             *j = count % 127;
             std::cout << count << " -> " << ( int32_t )*j
                       << std::endl;
             ++ j;
             ++ count;

           }

         }

       }

    }

    for ( i = 0; i < COUNT; i++ )
    {

      delete vectors[ i ];

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
