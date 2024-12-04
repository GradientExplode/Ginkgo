#ifndef _gkg_processing_machinelearning_SupportVectorMachine_h_
#define _gkg_processing_machinelearning_SupportVectorMachine_h_


#include <gkg-processing-machinelearning/MachineLearningCrossValidation.h>
#include <gkg-core-cppext/StdInt.h>

#include <vector>
#include <string>


namespace gkg
{


class SupportVectorMachineImplementation;
class MachineLearningProblem;
class MachineLearningData;
class Matrix;
class Vector;


class SupportVectorMachine
{

  public:

    enum SVMType
    {

      C_SVC = 0,
      nu_SVC,
      one_class_SVM,
      epsilon_SVR,
      nu_SVR

    };

    enum KernelType
    {

      Linear = 0,
      Polynomial,
      RadialBasisFunction,
      Sigmoid

    };

    SupportVectorMachine( SVMType type = C_SVC,
                          KernelType kernel = RadialBasisFunction,
                          int32_t degree = 3,
                          double gamma = 0.0,
                          double coef0 = 0.0,
                          double cost = 1.0,
                          double nu = 0.5,
                          double epsilonSVR = 0.1,
                          double epsilonTermination = 0.001,
                          int32_t cacheSizeMB = 100,
                          int32_t shrinking = 1 );
    virtual ~SupportVectorMachine();

    void setWeights( const std::vector< int32_t >& indices,
                     const std::vector< double >& weights );

    void train( const MachineLearningProblem& problem );
    MachineLearningCrossValidation crossValidation( 
                                          const MachineLearningProblem& problem,
                                          int32_t foldCount );
    void predict( const MachineLearningData& data,
                  Vector& labels,
                  Matrix* probabilities = 0 ) const;

    void write( const std::string& fileName ) const;
    void read( const std::string& fileName );

    SupportVectorMachineImplementation* getImplementation() const;

  protected:

    SupportVectorMachineImplementation* _supportVectorMachineImplementation;

};


}


#endif
