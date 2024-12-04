#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/SupportVectorMachineImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningSelector.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>
#include <limits>


gkg::SupportVectorMachine::SupportVectorMachine( 
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
                                   int32_t shrinking )
{

  try
  {

    _supportVectorMachineImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                   createSupportVectorMachineImplementation( this, 
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
  GKG_CATCH( "gkg::SupportVectorMachine::SupportVectorMachine( "
             "gkg::SupportVectorMachine::SVMType type, "
             "gkg::SupportVectorMachine::KernelType kernel, "
             "int32_t degree, double gamma, double coef0, double cost, "
             "double nu, double epsilonSVR, double epsilonTermination, "
             "int32_t cacheSizeMB, int32_t shrinking )" );

}


gkg::SupportVectorMachine::~SupportVectorMachine()
{

  delete _supportVectorMachineImplementation;

}


void gkg::SupportVectorMachine::setWeights(
                                          const std::vector< int32_t >& indices,
                                          const std::vector< double >& weights )
{

  try
  {

    _supportVectorMachineImplementation->setWeights( indices, weights );

  }
  GKG_CATCH( "void gkg::SupportVectorMachine::setWeights( "
             "const std::vector< int32_t >& indices, "
             "const std::vector< double >& weights )" );

}


void gkg::SupportVectorMachine::train( 
                                    const gkg::MachineLearningProblem& problem )
{

  try
  {

    _supportVectorMachineImplementation->train( problem );

  }
  GKG_CATCH( "void gkg::SupportVectorMachine::train( "
             "const gkg::MachineLearningProblem& problem )" );

}


gkg::MachineLearningCrossValidation gkg::SupportVectorMachine::crossValidation(
                                     const gkg::MachineLearningProblem& problem,
                                     int32_t foldCount )
{

  try
  {

    return _supportVectorMachineImplementation->crossValidation( problem,
                                                                 foldCount );

  }
  GKG_CATCH( "gkg::MachineLearningCrossValidation "
             "gkg::SupportVectorMachine::crossValidation( "
             "const gkg::MachineLearningProblem& problem, "
             "int32_t foldCount )" );

}


void gkg::SupportVectorMachine::predict( const gkg::MachineLearningData& data,
                                         gkg::Vector& labels,
                                         gkg::Matrix* probabilities ) const
{

  try
  {

    _supportVectorMachineImplementation->predict( data, labels, probabilities );

  }
  GKG_CATCH( "void gkg::SupportVectorMachine::predict( "
             "const gkg::MachineLearningData& data, "
             "gkg::Vector& labels, "
             "gkg::Matrix* probabilities ) const" );

}


void gkg::SupportVectorMachine::write( const std::string& fileName ) const
{

  try
  {

    _supportVectorMachineImplementation->write( fileName );

  }
  GKG_CATCH( "void gkg::SupportVectorMachine::write( "
             "const std::string& fileName ) const" );

}


void gkg::SupportVectorMachine::read( const std::string& fileName )
{

  try
  {

    _supportVectorMachineImplementation->read( fileName );

  }
  GKG_CATCH( "void gkg::SupportVectorMachine::read( "
             "const std::string& fileName )" );

}




gkg::SupportVectorMachineImplementation*
gkg::SupportVectorMachine::getImplementation() const
{

  try
  {

    return _supportVectorMachineImplementation;

  }
  GKG_CATCH( "gkg::SupportVectorMachineImplementation* "
             "gkg::SupportVectorMachine::getImplementation() const" );

}
