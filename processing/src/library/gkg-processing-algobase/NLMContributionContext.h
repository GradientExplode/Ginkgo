#ifndef _gkg_processing_algobase_NLMContributionContext_h_
#define _gkg_processing_algobase_NLMContributionContext_h_


#include <gkg-communication-thread/LoopContext.h>


namespace gkg
{


template < class T > class Volume;
class NLMNoiseModel;


template < class IN, class OUT >
class NLMContributionContext : public LoopContext< int32_t >
{

  public:

    NLMContributionContext( const Volume< IN >& in,
                            Volume< OUT >& out,
                            float* volTmp,
                            float* M,
                            float* Z,
                            const NLMNoiseModel& noiseModel,
                            const int32_t& t  );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    float* _tmp;
    float* _M;
    float* _Z;
    const NLMNoiseModel& _noiseModel;
    const int32_t& _t;

};


}


#endif
