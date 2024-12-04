#ifndef _gkg_processing_algobase_StatisticAnalyzerMinimumContext_h_
#define _gkg_processing_algobase_StatisticAnalyzerMinimumContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class StatisticAnalyzerMinimumContext : public LoopContext< int32_t >
{

  public:

    StatisticAnalyzerMinimumContext( const Volume< T >& volume, T& minimum );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< T >& _volume;
    T& _minimum;

};


}


#endif
