#ifndef _gkg_processing_plugin_svm_LibsvmMachineLearningImplementationFactory_h_
#define _gkg_processing_plugin_svm_LibsvmMachineLearningImplementationFactory_h_


#include <gkg-processing-machinelearning/MachineLearningImplementationFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>


namespace gkg
{


class LibsvmMachineLearningImplementationFactory : 
           public MachineLearningImplementationFactory,
           public SingletonAccessor< LibsvmMachineLearningImplementationFactory,
                                     MachineLearningImplementationFactory >
{

  public:

    ~LibsvmMachineLearningImplementationFactory();

    SupportVectorMachineImplementation*
      createSupportVectorMachineImplementation( 
        SupportVectorMachine* supportVectorMachine,
        SupportVectorMachine::SVMType type,
        SupportVectorMachine::KernelType kernel,
        int32_t degree,
        double gamma,
        double coef0,
        double cost,
        double nu,
        double epsilonSVR,
        double epsilonTermination,
        int32_t cacheSizeMB,
        int32_t shrinking ) const;

    MachineLearningDataImplementation*
      createMachineLearningDataImplementation(
        MachineLearningData* machineLearningData,
        SparseMatrix& data ) const;
    MachineLearningDataImplementation*
      createMachineLearningDataImplementation(
        MachineLearningData* machineLearningData,
        Matrix& data ) const;

    MachineLearningProblemImplementation*
      createMachineLearningProblemImplementation(
        MachineLearningProblem* machineLearningProblem,
        MachineLearningData& machineLearningData,
        Vector& labels ) const;

  protected:

    friend class Singleton< LibsvmMachineLearningImplementationFactory >;

    LibsvmMachineLearningImplementationFactory();

};


}


#endif
