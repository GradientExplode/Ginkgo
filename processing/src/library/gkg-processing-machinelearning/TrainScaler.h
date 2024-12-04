#ifndef _gkg_processing_machinelearning_TrainScaler_h_
#define _gkg_processing_machinelearning_TrainScaler_h_


#include <gkg-processing-machinelearning/MachineLearningScaler.h>


namespace gkg
{


class TrainScaler : public MachineLearningScaler
{

  public:

    TrainScaler( const Matrix& inMinMax,  
                 double outMin = -1.0,
                 double outMax = 1.0 );
    TrainScaler( double outMin = -1, double outMax = 1.0 );
    TrainScaler( const MachineLearningScaler& other );

    void scale( SparseMatrix& data );
    void scale( Matrix& data );

};


}


#endif
