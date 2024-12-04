#ifndef _gkg_processing_algobase_StatisticAnalyzerMinimumContext_i_h_
#define _gkg_processing_algobase_StatisticAnalyzerMinimumContext_i_h_


#include <gkg-processing-algobase/StatisticAnalyzerMinimumContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::StatisticAnalyzerMinimumContext< T >::StatisticAnalyzerMinimumContext(
                                                      const Volume< T >& volume,
                                                      T& minimum )
                                         : gkg::LoopContext< int32_t >(),
                                           _volume( volume ),
                                           _minimum( minimum )
{
}


template < class T >
void gkg::StatisticAnalyzerMinimumContext< T >::doIt( int32_t startIndex, 
                                                      int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;
    int32_t sizeX = _volume.getSizeX();
    int32_t sizeY = _volume.getSizeY();
    int32_t sizeT = _volume.getSizeT();
    int32_t x, y, z, t;
    T minimum = _minimum;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = startIndex; z < stopIndex; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            T value = _volume( x, y, z, t );

            if ( value < minimum )
            {

              minimum = value;

            }

          }

        }

      }

    }

    lock();

    if ( minimum < _minimum )
    {

      _minimum = minimum;

    }

    unlock();

  }
  GKG_CATCH( "template < class T > "
             "gkg::StatisticAnalyzerMinimumContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
