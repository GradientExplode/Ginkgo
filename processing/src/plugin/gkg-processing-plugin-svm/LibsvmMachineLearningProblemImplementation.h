#ifndef _gkg_processing_plugin_svm_LibsvmMachineLearningProblemImplementation_h_
#define _gkg_processing_plugin_svm_LibsvmMachineLearningProblemImplementation_h_


#include <gkg-processing-machinelearning/MachineLearningProblemImplementation.h>

#include <libsvm/svm.h>


namespace gkg
{


class LibsvmMachineLearningProblemImplementation : 
                                     public MachineLearningProblemImplementation
{

  public:

    LibsvmMachineLearningProblemImplementation(
                                 MachineLearningProblem* machineLearningProblem,
                                 MachineLearningData& machineLearningData,
                                 Vector& labels );
    virtual ~LibsvmMachineLearningProblemImplementation();

    const svm_problem& getLibsvmProblem() const;

    static LibsvmMachineLearningProblemImplementation* getImplementation(
                               MachineLearningProblem* machineLearningProblem );

  private:

    svm_problem _libsvmProblem;

};


}


#endif
