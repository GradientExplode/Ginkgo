#ifndef _gkg_processing_machinelearning_SupportVectorMachineImplementation_h_
#define _gkg_processing_machinelearning_SupportVectorMachineImplementation_h_


#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-core-cppext/StdInt.h>

#include <vector>
#include <map>


namespace gkg
{


class SupportVectorMachineImplementation
{

  public:

    virtual ~SupportVectorMachineImplementation();

    virtual SupportVectorMachine* getTarget() const;

    virtual void setWeights( const std::vector< int32_t >& indices,
                             const std::vector< double >& weights ) = 0;

    virtual void train( const MachineLearningProblem& problem ) = 0;
    virtual MachineLearningCrossValidation crossValidation( 
                                          const MachineLearningProblem& problem,
                                          int32_t foldCount ) = 0;
    virtual void predict( const MachineLearningData& data,
                          Vector& labels,
                          Matrix* probabilities ) const = 0;

    virtual void write( const std::string& fileName ) const = 0;
    virtual void read( const std::string& fileName ) = 0;

  protected:

    SupportVectorMachineImplementation(
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
                                     int32_t shrinking );

    SupportVectorMachine* _target;

};


}


#endif
