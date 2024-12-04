#ifndef _gkg_processing_numericalanalysis_Kalman_h_
#define _gkg_processing_numericalanalysis_Kalman_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/KFFunction.h>
#include <gkg-core-pattern/RCPointer.h>



namespace gkg
{


//
// Kalman's iterative solver for Y = A * X + E
//   Y : ( dataCount x 1 ) vector of measures 
//   A : ( dataCount x regressorCount ) matrix of design
//   X : ( regressorCount x 1 ) vector of regressors
//   E : ( dataCount x 1 ) vector of errors
//

class Kalman
{

  public:

    struct Parameters
    {

      Parameters( int32_t regressorCount );
      Parameters( const Parameters& other );

      void sanityCheck( int32_t regressorCount ) const;

      Vector X;
      Matrix varianceOfX;
      double sumOfSquareResiduals;
      double squareScaleParameter;

    };

    Kalman( RCPointer< KFFunction > K );
    virtual ~Kalman();

    virtual void setSubDesignMatrix( const Vector& subDesignMatrix );

    int32_t getRegressorCount() const;
    const Vector& getSubDesignMatrix() const;

    virtual void iterate( Parameters& parameters,
                          double y,
                          int32_t time );

  protected:

    void update( Parameters& parameters,
                 double innovatin,
                 double inverseVarianceOfY,
                 Vector& covarianceOfAY,
                 int32_t time );

    int32_t _regressorCount;

    RCPointer< KFFunction > _K;
    Vector _a;

};


}


#endif
