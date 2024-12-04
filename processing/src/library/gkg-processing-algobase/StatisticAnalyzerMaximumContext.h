#ifndef _gkg_processing_algobase_StatisticAnalyzerMaximumContext_h_
#define _gkg_processing_algobase_StatisticAnalyzerMaximumContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class StatisticAnalyzerMaximumContext : public LoopContext< int32_t >
{

  public:

    StatisticAnalyzerMaximumContext( const Volume< T >& volume, T& maximum );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< T >& _volume;
    T& _maximum;

};


}


#endif
