#ifndef _gkg_fmri_statistics_StatisticalFunctionScoreMaskContext_h_
#define _gkg_fmri_statistics_StatisticalFunctionScoreMaskContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>

#include <vector>


namespace gkg
{


class GLMStatisticalFunction;


class StatisticalFunctionScoreMaskContext : public LoopContext< int32_t >
{

  public:

    StatisticalFunctionScoreMaskContext(
                                GLMStatisticalFunction& statisticalFunction,
                                const std::vector< Vector3d< int32_t > >& sites,
                                int32_t contrastCount,
                                const Volume< float >& volumeOfEffects,
                                const Volume< float >& volumeOfEffectVariances,
                                Volume< float >& statistics,
                                int32_t statisticCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    GLMStatisticalFunction& _statisticalFunction;
    const std::vector< Vector3d< int32_t > >& _sites;
    int32_t _contrastCount;
    const Volume< float >& _volumeOfEffects;
    const Volume< float >& _volumeOfEffectVariances;
    Volume< float >& _statistics;
    int32_t _statisticCount;

};


}


#endif
