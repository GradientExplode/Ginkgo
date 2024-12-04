#ifndef _gkg_processing_histogram_HistogramContext_h_
#define _gkg_processing_histogram_HistogramContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>

#include <set>


namespace gkg
{


template < class T >
class HistogramContext : public LoopContext< int32_t >
{

  public:

    HistogramContext( const Volume< T >& volume,
                      std::vector< double >& histogram,
                      double lowerBoundary,
                      double increment );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const T* _ptr0;
    std::vector< double >& _histogram;
    const double _lowerBoundary;
    const double _increment;

};


}


#endif
