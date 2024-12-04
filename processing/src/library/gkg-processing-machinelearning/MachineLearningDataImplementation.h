#ifndef _gkg_processing_machinelearning_MachineLearningDataImplementation_h_
#define _gkg_processing_machinelearning_MachineLearningDataImplementation_h_


#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class MachineLearningData;


class MachineLearningDataImplementation
{

  public:

    virtual ~MachineLearningDataImplementation();

    virtual MachineLearningData* getTarget() const;

    virtual int32_t getMeasureCount() const = 0;
    virtual int32_t getMaximumIndex() const = 0;

  protected:

    MachineLearningDataImplementation( MachineLearningData* machineLearningData,
                  		       SparseMatrix& data );
    MachineLearningDataImplementation( MachineLearningData* machineLearningData,
                  		       Matrix& data );

    MachineLearningData* _target;

};


}


#endif
