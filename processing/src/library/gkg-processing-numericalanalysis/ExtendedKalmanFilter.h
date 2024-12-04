#ifndef _gkg_processing_numericalanalysis_ExtendedKalmanFilter_h_
#define _gkg_processing_numericalanalysis_ExtendedKalmanFilter_h_


#include <gkg-processing-numericalanalysis/Kalman.h>
#include <gkg-processing-numericalanalysis/KFFunction.h>


namespace gkg
{


class ExtendedKalmanFilter : public Kalman
{

  public:

    ExtendedKalmanFilter( RCPointer< KFFunction > K );
    
    void iterate( Parameters& parameters, double y, int32_t t );

};


}


#endif
