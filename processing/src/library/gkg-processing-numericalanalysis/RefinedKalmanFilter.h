#ifndef _gkg_processing_numericalanalysis_RefinedKalmanFilter_h_
#define _gkg_processing_numericalanalysis_RefinedKalmanFilter_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>


#define GKG_REFINED_KALMAN_ITERATION_COUNT      1


namespace gkg
{


//
// refined Kalman's iterative solver for Y = A * X + E
//   Y : ( dataCount x 1 ) vector of measures 
//   A : ( dataCount x regressorCount ) matrix of design
//   X : ( regressorCount x 1 ) vector of regressors
//   E : ( dataCount x 1 ) vector of errors
//

class RefinedKalmanFilter
{

  public:


    struct Parameters
    {

      Parameters( int32_t regressorCount );
      Parameters( const Parameters& other );

      void sanityCheck( int32_t regressorCount ) const;

      Kalman::Parameters kalmanFilterParameters;

      Matrix hessianOfSSR;       // sum of square residuals
      double sumOfPairProducts;
      Vector gradientOfSPP;      // SPP = sum of pair products
      Matrix hessianOfSPP;

      double autocorrelation;

    };

    RefinedKalmanFilter( const Matrix& A,
                         int32_t iterationCount =
                                           GKG_REFINED_KALMAN_ITERATION_COUNT );
    virtual ~RefinedKalmanFilter();

    void iterate( Parameters& parameters,
                  double y,
                  double previousY,
                  int32_t time );

  protected:

    KalmanFilter _kalmanFilter;

    int32_t _iterationCount;

    Vector _previousA;
    Vector _previousX;
    Vector _deltaX;

};


}


#endif
