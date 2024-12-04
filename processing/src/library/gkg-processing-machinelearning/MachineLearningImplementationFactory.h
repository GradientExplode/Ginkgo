#ifndef _gkg_processing_machinelearning_MachineLearningImplementationFactory_h_
#define _gkg_processing_machinelearning_MachineLearningImplementationFactory_h_


#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class MachineLearningImplementation;
class MachineLearningDataImplementation;
class MachineLearningProblemImplementation;


class MachineLearningImplementationFactory
{

  public:

    virtual ~MachineLearningImplementationFactory();

    virtual SupportVectorMachineImplementation*
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
        int32_t shrinking ) const = 0;

    virtual MachineLearningDataImplementation*
      createMachineLearningDataImplementation(
        MachineLearningData* machineLearningData,
        SparseMatrix& data ) const = 0;
    virtual MachineLearningDataImplementation*
      createMachineLearningDataImplementation(
        MachineLearningData* machineLearningData,
        Matrix& data ) const = 0;

    virtual MachineLearningProblemImplementation*
      createMachineLearningProblemImplementation(
        MachineLearningProblem* machineLearningProblem,
        MachineLearningData& machineLearningData,
        Vector& labels ) const = 0;

  protected:

    MachineLearningImplementationFactory();

};


}


#endif
