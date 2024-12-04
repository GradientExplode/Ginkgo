#ifndef _gkg_processing_numericalanalysis_ParallelKalmanFilter_h_
#define _gkg_processing_numericalanalysis_ParallelKalmanFilter_h_


#include <gkg-processing-numericalanalysis/KalmanFilter.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class ParallelKalmanFilter : public KalmanFilter
{

  public:

    ParallelKalmanFilter( const Matrix& A, int32_t gaussianCount );

    void iterate( Parameters& parameters,
                  Vector& gaussianMixtureNoise,
                  double y,
                  int32_t time );
		  
  protected:
  
    int32_t _gaussianCount;
    double _varianceOfY;
    Vector _y;
    Vector _covariance;
    Vector _weight;

};


}


#endif
