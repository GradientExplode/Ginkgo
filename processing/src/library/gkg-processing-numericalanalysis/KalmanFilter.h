#ifndef _gkg_processing_numericalanalysis_KalmanFilter_h_
#define _gkg_processing_numericalanalysis_KalmanFilter_h_


#include <gkg-processing-numericalanalysis/Kalman.h>



namespace gkg
{


//
// Kalman's iterative solver for Y = A * X + E
//   Y : ( dataCount x 1 ) vector of measures 
//   A : ( dataCount x regressorCount ) matrix of design
//   X : ( regressorCount x 1 ) vector of regressors
//   E : ( dataCount x 1 ) vector of errors
//

class KalmanFilter : public Kalman
{

  public:


    KalmanFilter( const Matrix& A );

    int32_t getDataCount() const;
    const Matrix& getDesignMatrix() const;

    void setSubDesignMatrix( int32_t time );
    void iterate( Parameters& parameters, 
                  double y, 
                  int32_t time, 
                  bool doUpdate = true );

  protected:

    int32_t _dataCount;
    Matrix _A;

};


}


#endif
