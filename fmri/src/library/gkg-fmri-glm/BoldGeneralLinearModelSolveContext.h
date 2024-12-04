#ifndef _gkg_fmri_glm_BoldGeneralLinearModelSolveContext_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelSolveContext_h_

#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>

#include <vector>


namespace gkg
{


template < class T >
class BoldGeneralLinearModelSolveContext : public LoopContext< int32_t >
{

  public:

    BoldGeneralLinearModelSolveContext(
                                KalmanFilter& kalmanFilter,
                                const Volume< T >& volume,
                                Volume< Kalman::Parameters* >& kalmanLut,
                                const std::vector< Vector3d< int32_t > >& sites,
                                int32_t t,
                                int32_t dummyScanCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    KalmanFilter& _kalmanFilter;
    const Volume< T >& _volume;
    Volume< Kalman::Parameters* >& _kalmanLut;
    const std::vector< Vector3d< int32_t > >& _sites;
    int32_t _t;
    int32_t _dummyScanCount;

};


}


#endif
