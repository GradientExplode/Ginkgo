#ifndef _gkg_processing_machinelearning_PredictScaler_h_
#define _gkg_processing_machinelearning_PredictScaler_h_


#include <gkg-processing-machinelearning/MachineLearningScaler.h>


namespace gkg
{


class PredictScaler : public MachineLearningScaler
{

  public:

    PredictScaler( const Matrix& inMinMax,  
                   double outMin = -1.0,
                   double outMax = 1.0 );
    PredictScaler( double outMin = -1, double outMax = 1.0 );
    PredictScaler( const MachineLearningScaler& other );
    
    PredictScaler& operator = ( const PredictScaler& other );

    void scale( SparseMatrix& data );
    void scale( Matrix& data );

};


}


#endif
