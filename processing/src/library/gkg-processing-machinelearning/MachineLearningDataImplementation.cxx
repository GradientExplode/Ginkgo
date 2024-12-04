#include <gkg-processing-machinelearning/MachineLearningDataImplementation.h>


gkg::MachineLearningDataImplementation::MachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                  		  gkg::SparseMatrix& /* data */ )
                                      : _target( machineLearningData )
{
}


gkg::MachineLearningDataImplementation::MachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                  		  gkg::Matrix& /* data */ )
                                      : _target( machineLearningData )
{
}


gkg::MachineLearningDataImplementation::~MachineLearningDataImplementation()
{
}


gkg::MachineLearningData* 
gkg::MachineLearningDataImplementation::getTarget() const
{

  return _target;

}
