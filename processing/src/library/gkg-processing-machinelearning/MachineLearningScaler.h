#ifndef _gkg_processing_machinelearning_MachineLearningScaler_h_
#define _gkg_processing_machinelearning_MachineLearningScaler_h_


#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class SparseMatrix;


class MachineLearningScaler
{

  public:

    MachineLearningScaler( const Matrix& inMinMax, 
                           double outMin = -1.0,
                           double outMax = 1.0 );
    MachineLearningScaler( double outMin = -1, double outMax = 1.0 );
    MachineLearningScaler( const MachineLearningScaler& other );
    virtual ~MachineLearningScaler();
    
    MachineLearningScaler& operator = ( const MachineLearningScaler& other );

    virtual void scale( SparseMatrix& data ) = 0;
    virtual void scale( Matrix& data ) = 0;

    const Matrix& getScaleMatrix() const;

  protected:

    Matrix _scale;
    double _outMin;
    double _outMax;
    
    void copy( const MachineLearningScaler& other );

};


}


#endif
