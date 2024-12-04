#ifndef _gkg_processing_numericalanalysis_UnscentedKalmanFilter_h_
#define _gkg_processing_numericalanalysis_UnscentedKalmanFilter_h_


#include <gkg-processing-numericalanalysis/Kalman.h>
#include <gkg-processing-numericalanalysis/KFFunction.h>

#include <vector>


namespace gkg
{


class UnscentedKalmanFilter : public Kalman
{

  public:

    UnscentedKalmanFilter( RCPointer< KFFunction > K, double k = 0.01 );

    void iterate( Parameters& parameters, double y, int32_t t );

  protected:

    void getSigmaPoints( Parameters& parameters, Matrix& sp );

    double _scale;
    int32_t _sigmaPointCount;
    Vector _w;

};


}


#endif
