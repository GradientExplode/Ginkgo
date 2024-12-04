#ifndef _gkg_processing_plugin_svm_LibsvmSupportVectorMachineImplementation_h_
#define _gkg_processing_plugin_svm_LibsvmSupportVectorMachineImplementation_h_


#include <gkg-processing-machinelearning/SupportVectorMachineImplementation.h>

#include <libsvm/svm.h>


namespace gkg
{


class LibsvmSupportVectorMachineImplementation :
                                       public SupportVectorMachineImplementation
{

  public:

    LibsvmSupportVectorMachineImplementation(
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
    ~LibsvmSupportVectorMachineImplementation();

    void setWeights( const std::vector< int32_t >& indices,
                     const std::vector< double >& weights );

    void train( const MachineLearningProblem& machineLearningProblem );
    MachineLearningCrossValidation crossValidation( 
                           const MachineLearningProblem& machineLearningProblem,
                           int32_t foldCount );
    void predict( const MachineLearningData& data,
                  Vector& labels,
                  Matrix* probabilities ) const;

    void write( const std::string& fileName ) const;
    void read( const std::string& fileName );

  private:

    static void svmPrintFunc( const char* );

    svm_parameter _parameters;
    svm_model* _model;

};


}


#endif
