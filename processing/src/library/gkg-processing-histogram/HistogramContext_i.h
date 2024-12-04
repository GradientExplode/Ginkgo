#ifndef _gkg_processing_histogram_HistogramContext_i_h_
#define _gkg_processing_histogram_HistogramContext_i_h_


#include <gkg-processing-histogram/HistogramContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>


template < class T >
gkg::HistogramContext< T >::HistogramContext( const gkg::Volume< T >& volume,
                                              std::vector< double >& histogram,
                                              double lowerBoundary,
                                              double increment )
                          : gkg::LoopContext< int32_t >(),
                            _ptr0( &volume( 0 ) ),
                            _histogram( histogram ),
                            _lowerBoundary( lowerBoundary ),
                            _increment( increment )
{
}

template < class T >
void gkg::HistogramContext< T >::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    const T* ptr = _ptr0 + startIndex;
    int32_t levelCount = ( int32_t )_histogram.size();
    double h[ levelCount ];

    std::memset( h, 0, levelCount * sizeof( double ) );

    while ( count-- )
    {

      int32_t level = ( int32_t )( ( *ptr++ - _lowerBoundary ) / _increment );
      h[ level ] ++;

    }

    lock();

    double* i = h;
    std::vector< double >::iterator
      j = _histogram.begin();

    while ( levelCount-- )
    {

      *j += *i++;
      ++j;

    }

    unlock();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HistogramContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
