#include <gkg-processing-machinelearning/MachineLearningProblemImplementation.h>


gkg::MachineLearningProblemImplementation::MachineLearningProblemImplementation( 
                            gkg::MachineLearningProblem* machineLearningProblem,
                            gkg::MachineLearningData& /* machineLearningData */,
                            gkg::Vector& /* labels */ )
                                      : _target( machineLearningProblem )
{
}


gkg::MachineLearningProblemImplementation::
                                         ~MachineLearningProblemImplementation()
{
}


gkg::MachineLearningProblem* 
gkg::MachineLearningProblemImplementation::getTarget() const
{

  return _target;

}
