#include <gkg-processing-plugin-svm/LibsvmMachineLearningImplementationFactory.h>
#include <gkg-processing-plugin-svm/LibsvmSupportVectorMachineImplementation.h>
#include <gkg-processing-plugin-svm/LibsvmMachineLearningDataImplementation.h>
#include <gkg-processing-plugin-svm/LibsvmMachineLearningProblemImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningSelector.h>
#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-core-exception/Exception.h>


ForceSingletonInstanciation( gkg::LibsvmMachineLearningImplementationFactory,
                             libsvm );


gkg::LibsvmMachineLearningImplementationFactory::
                                    LibsvmMachineLearningImplementationFactory()
           : gkg::MachineLearningImplementationFactory(),
             gkg::SingletonAccessor< LibsvmMachineLearningImplementationFactory,
                                     MachineLearningImplementationFactory >()
{

  try
  {

    gkg::MachineLearningSelector::getInstance().registerFactory(
                "libsvm",
                &gkg::LibsvmMachineLearningImplementationFactory::getAccessor );

  }
  GKG_CATCH( "gkg::LibSVMImplementationFactory::"
             "LibSVMImplementationFactory()" );

}


gkg::LibsvmMachineLearningImplementationFactory::
                                   ~LibsvmMachineLearningImplementationFactory()
{
}


gkg::SupportVectorMachineImplementation* 
gkg::LibsvmMachineLearningImplementationFactory::
                                       createSupportVectorMachineImplementation(
                                gkg::SupportVectorMachine* supportVectorMachine,
                                gkg::SupportVectorMachine::SVMType type,
                                gkg::SupportVectorMachine::KernelType kernel,
                                int32_t degree,
                                double gamma,
                                double coef0,
                                double cost,
                                double nu,
                                double epsilonSVR,
                                double epsilonTermination,
                                int32_t cacheSizeMB,
                                int32_t shrinking ) const
{

  try
  {

    return new gkg::LibsvmSupportVectorMachineImplementation(
                                          supportVectorMachine,
                                          type,
                                          kernel,
                                          degree,
                                          gamma,
                                          coef0,
                                          cost,
                                          nu,
                                          epsilonSVR,
                                          epsilonTermination,
                                          cacheSizeMB,
                                          shrinking );

  }
  GKG_CATCH( "gkg::SupportVectorMachineImplementation* "
             "gkg::LibsvmMachineLearningImplementationFactory::"
             "createSupportVectorMachineImplementation( "
             "gkg::SupportVectorMachine* supportVectorMachine, "
             "gkg::SupportVectorMachine::SVMType type, "
             "gkg::SupportVectorMachine::KernelType kernel, "
             "int32_t degree, "
             "double gamma, "
             "double coef0, "
             "double cost, "
             "double nu, "
             "double epsilonSVR, "
             "double epsilonTermination, "
             "int32_t cacheSizeMB, "
             "int32_t shrinking ) const" );

}


gkg::MachineLearningDataImplementation*
gkg::LibsvmMachineLearningImplementationFactory::
                                        createMachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                                  gkg::SparseMatrix& data ) const
{

  try
  {

    return new gkg::LibsvmMachineLearningDataImplementation( 
                                                            machineLearningData,
                                                            data );

  }
  GKG_CATCH( "gkg::MachineLearningDataImplementation* "
             "gkg::LibsvmMachineLearningImplementationFactory::"
             "createMachineLearningDataImplementation( "
             "gkg::MachineLearningData* machineLearningData, "
             "gkg::SparseMatrix& data ) const" );

}


gkg::MachineLearningDataImplementation*
gkg::LibsvmMachineLearningImplementationFactory::
                                        createMachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                                  gkg::Matrix& data ) const
{

  try
  {

    return new gkg::LibsvmMachineLearningDataImplementation( 
                                                            machineLearningData,
                                                            data );

  }
  GKG_CATCH( "gkg::MachineLearningDataImplementation* "
             "gkg::LibsvmMachineLearningImplementationFactory::"
             "createMachineLearningDataImplementation( "
             "gkg::MachineLearningData* machineLearningData, "
             "gkg::Matrix& data ) const" );

}


gkg::MachineLearningProblemImplementation*
gkg::LibsvmMachineLearningImplementationFactory::
                                     createMachineLearningProblemImplementation(
                            gkg::MachineLearningProblem* machineLearningProblem,
                            gkg::MachineLearningData& machineLearningData,
                            gkg::Vector& labels ) const
{

  try
  {

    return new gkg::LibsvmMachineLearningProblemImplementation( 
                                                       machineLearningProblem,
                                                       machineLearningData,
                                                       labels );

  }
  GKG_CATCH( "gkg::MachineLearningProblemImplementation* "
             "gkg::LibsvmMachineLearningImplementationFactory::"
             "createMachineLearningProblemImplementation( "
             "gkg::MachineLearningProblem* machineLearningProblem, "
             "gkg::MachineLearningData& machineLearningData, "
             "gkg::Vector& labels ) const" );

}
