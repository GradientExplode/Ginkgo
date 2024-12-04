#ifndef _gkg_fmri_statistics_StatisticalFunctionScoreContext_h_
#define _gkg_fmri_statistics_StatisticalFunctionScoreContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class GLMStatisticalFunction;


class StatisticalFunctionScoreContext : public LoopContext< int32_t >
{

  public:

    StatisticalFunctionScoreContext(
                                 GLMStatisticalFunction& statisticalFunction,
                                 int32_t contrastCount,
                                 const Volume< float >& volumeOfEffects,
                                 const Volume< float >& volumeOfEffectVariances,
                                 Volume< float >& statistics,
                                 int32_t statisticCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    GLMStatisticalFunction& _statisticalFunction;
    int32_t _contrastCount;
    const Volume< float >& _volumeOfEffects;
    const Volume< float >& _volumeOfEffectVariances;
    Volume< float >& _statistics;
    int32_t _statisticCount;

};


}


#endif
