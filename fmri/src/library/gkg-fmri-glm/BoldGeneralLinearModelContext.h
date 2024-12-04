#ifndef _gkg_fmri_glm_BoldGeneralLinearModelContext_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelContext_h_

#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>


namespace gkg
{


template < class T >
class BoldGeneralLinearModelContext : public LoopContext< int32_t >
{

  public:

    BoldGeneralLinearModelContext(
                                KalmanFilter& kalmanFilter,
                                const Volume< T >& volume,
                                Volume< Kalman::Parameters* >& kalmanLut,
                                const std::vector< Vector3d< int32_t > >& sites,
                                int32_t t );

    void doIt( int32_t startIndex, int32_t count );

  private:

    KalmanFilter& _kalmanFilter;
    const Volume< T >& _volume;
    Volume< Kalman::Parameters* >& _kalmanLut;
    const std::vector< Vector3d< int32_t > >& _sites;
    int32_t _t;

};


}


#endif
