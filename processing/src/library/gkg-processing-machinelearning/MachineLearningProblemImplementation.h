#ifndef _gkg_processing_machinelearning_MachineLearningProblemImplementation_h_
#define _gkg_processing_machinelearning_MachineLearningProblemImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class MachineLearningProblem;
class MachineLearningData;
class Vector;


class MachineLearningProblemImplementation
{

  public:

    virtual ~MachineLearningProblemImplementation();

    virtual MachineLearningProblem* getTarget() const;

  protected:

    MachineLearningProblemImplementation(
                                 MachineLearningProblem* machineLearningProblem,
                                 MachineLearningData& machineLearningData,
                                 Vector& labels );

    MachineLearningProblem* _target;

};


}


#endif
