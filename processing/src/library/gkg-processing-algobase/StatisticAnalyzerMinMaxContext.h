#ifndef _gkg_processing_algobase_StatisticAnalyzerMinMaxContext_h_
#define _gkg_processing_algobase_StatisticAnalyzerMinMaxContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class StatisticAnalyzerMinMaxContext : public LoopContext< int32_t >
{

  public:

    StatisticAnalyzerMinMaxContext( const Volume< T >& volume,
                                    T& minimum,
                                    T& maximum );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< T >& _volume;
    T& _minimum;
    T& _maximum;

};


}


#endif
