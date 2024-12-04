#include <gkg-processing-plugin-svm/LibsvmSupportVectorMachineImplementation.h>
#include <gkg-processing-plugin-svm/LibsvmMachineLearningProblemImplementation.h>
#include <gkg-processing-plugin-svm/LibsvmMachineLearningDataImplementation.h>
#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>
#include <cmath>


gkg::LibsvmSupportVectorMachineImplementation::
                                       LibsvmSupportVectorMachineImplementation(
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
                                int32_t shrinking )
                : gkg::SupportVectorMachineImplementation( supportVectorMachine,
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
                                                           shrinking ),
                  _model( 0 )
{

  try
  {

    _parameters.svm_type = type;
    _parameters.kernel_type = kernel;
    _parameters.degree = degree;
    _parameters.gamma = gamma;
    _parameters.coef0 = coef0;
    _parameters.C = cost;
    _parameters.nu = nu;
    _parameters.cache_size = cacheSizeMB;
    _parameters.p = epsilonSVR;
    _parameters.eps = epsilonTermination;
    _parameters.shrinking = shrinking;
    _parameters.probability = 1; // 1: to get probabilities on output
    _parameters.nr_weight = 0;
    _parameters.weight_label = 0;
    _parameters.weight = 0;

    svm_set_print_string_function(
                  gkg::LibsvmSupportVectorMachineImplementation::svmPrintFunc );

  }
  GKG_CATCH( "gkg::LibsvmSupportVectorMachineImplementation::"
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
             "int32_t shrinking )" );

}


gkg::LibsvmSupportVectorMachineImplementation::
                                     ~LibsvmSupportVectorMachineImplementation()
{

  if ( _model )
  {

    svm_free_and_destroy_model( &_model );

    if ( _parameters.weight_label )
    {

      delete [] _parameters.weight_label;

    }
    if ( _parameters.weight )
    {

      delete [] _parameters.weight;

    }

  }

}


void gkg::LibsvmSupportVectorMachineImplementation::setWeights(
                                          const std::vector< int32_t >& indices,
                                          const std::vector< double >& weights )
{

  try
  {


    int32_t weightCount = ( int32_t )weights.size();
    if ( indices.size() != weights.size() )
    {

      throw std::runtime_error( "inconsistent indices and weights size(s)" );

    }

    if ( weightCount )
    {

      _parameters.nr_weight = weightCount;
      _parameters.weight_label = new int32_t[ weightCount ];
      _parameters.weight = new double[ weightCount ];

      int32_t index = 0;
      for ( index = 0; index < weightCount; index++ )
      {

        ( _parameters.weight_label )[ index ] = indices[ index ];
        ( _parameters.weight )[ index ] = weights[ index ];

      }

    }

  }
  GKG_CATCH( "void gkg::LibsvmSupportVectorMachineImplementation::setWeights( "
             "const std::vector< int32_t >& indices, "
             "const std::vector< double >& weights )" );

}


void gkg::LibsvmSupportVectorMachineImplementation::train( 
                     const gkg::MachineLearningProblem& machineLearningProblem )
{

  try
  {

    int32_t maximumIndex = machineLearningProblem.getMaximumIndex();
    const svm_problem& problem = 
      static_cast< gkg::LibsvmMachineLearningProblemImplementation* >(
               machineLearningProblem.getImplementation() )->getLibsvmProblem();

    // tuning the gamma parameter if not set; in general, we take the inverse
    // of the number of features
    if ( ( _parameters.gamma == 0.0 ) && ( maximumIndex > 0 ) )
    {

      _parameters.gamma = 1.0 / ( double )maximumIndex;

    }

    // in case read() or train() were previsouly called, reset the model
    if ( _model )
    {

      svm_free_and_destroy_model( &_model );

    }

    // checking the parameter set
    const char* errorMessage = svm_check_parameter( &problem, &_parameters );
    if ( errorMessage )
    {

      throw std::runtime_error( std::string( "SVM error : " ) + 
                                std::string( errorMessage ) );

    }

    // finally, launching the training
    _model = svm_train( &problem, &_parameters );

  }
  GKG_CATCH( "void gkg::LibsvmSupportVectorMachineImplementation::train( "
             "const gkg::MachineLearningProblem& problem )" );

}


gkg::MachineLearningCrossValidation
gkg::LibsvmSupportVectorMachineImplementation::crossValidation(
                      const gkg::MachineLearningProblem& machineLearningProblem,
                      int32_t foldCount )
{

  try
  {

    gkg::MachineLearningCrossValidation result;
    const svm_problem& problem = 
      static_cast< gkg::LibsvmMachineLearningProblemImplementation* >(
               machineLearningProblem.getImplementation() )->getLibsvmProblem();
    double* target = new double[ problem.l ];

    if ( target )
    {

      int32_t i, correct = 0;
      double l = double( problem.l );

      svm_cross_validation( &problem, &_parameters, foldCount, target );

      if ( ( _parameters.svm_type == EPSILON_SVR ) ||
           ( _parameters.svm_type == NU_SVR ) )
      {

        double error = 0.0;
        double sumv = 0.0, sumy = 0.0, sumvv = 0.0, sumyy = 0.0, sumvy = 0.0;

        for ( i = 0; i < problem.l; i++ )
        {

          double v = target[ i ];
          double y = problem.y[ i ];

          error += ( v - y ) * ( v - y );
          sumv += v;
          sumy += y;
          sumvv += v * v;
          sumyy += y * y;
          sumvy += v * y;

        }

        result.meanSquareError = error / l;
        result.squaredCorrelationCoefficient = 
          ( ( l * sumvy - sumv * sumy ) * ( l * sumvy - sumv * sumy ) ) /
          ( ( l * sumvv - sumv * sumv ) * ( l * sumyy - sumy * sumy ) );

      }
      else
      {

        for ( i = 0; i < problem.l; i++ )
        {

          if ( target[ i ] == problem.y[ i ] )
          {

            correct++;

          }

        }

        result.accuracy = 100.0 * double( correct ) / l;

      }

      delete target;

    }

    return result;

  }
  GKG_CATCH( "gkg::MachineLearningCrossValidation "
             "gkg::LibsvmSupportVectorMachineImplementation::crossValidation( "
             "const gkg::MachineLearningProblem& problem, "
             "int32_t foldCount )" );

}


void gkg::LibsvmSupportVectorMachineImplementation::predict(
                                           const gkg::MachineLearningData& data,
                                           gkg::Vector& labels,
                                           gkg::Matrix* probabilities ) const
{

  try
  {

    // sanity check
    if ( !_model )
    {

      throw std::runtime_error( "no model trained or read" );

    }

    // collecting the class count
    int32_t classCount = svm_get_nr_class( _model );

    // collecting the number of data to classify
    int32_t dataCount = data.getMeasureCount();

    // reallocating the labels accordingly
    labels.reallocate( dataCount );
    labels.setZero();

    // if probability matrix is provided, reallocate it adequately
    gkg::Vector vectorOfProbabilities;
    if ( probabilities )
    {

      probabilities->reallocate( dataCount, classCount );
      probabilities->setZero();

      vectorOfProbabilities.reallocate( classCount );

    }

    // collecting the SVM type
    int32_t svmType = svm_get_svm_type( _model );

    // getting the data
    svm_node** x = static_cast< gkg::LibsvmMachineLearningDataImplementation* >(
                                    data.getImplementation() )->getLibsvmData();

    // looping over the data
    int32_t dataIndex = 0;
    for ( dataIndex = 0; dataIndex < dataCount; dataIndex++ )
    {

      if ( probabilities && ( ( svmType == C_SVC ) || ( svmType==NU_SVC ) ) )
      {


        labels( dataIndex ) = svm_predict_probability(
                                                  _model,
                                                  x[ dataIndex ],
                                                  &vectorOfProbabilities( 0 ) );

        probabilities->setRow( dataIndex, vectorOfProbabilities );

      }
      else
      {

        labels( dataIndex ) = svm_predict( _model, x[ dataIndex ] );

      }

    }

  }
  GKG_CATCH( "void gkg::LibsvmSupportVectorMachineImplementation::predict( "
             "const gkg::MachineLearningData& data, "
             "gkg::Vector& labels, "
             "gkg::Matrix* probabilities ) const" );

}


void gkg::LibsvmSupportVectorMachineImplementation::write(
                                             const std::string& fileName ) const
{

  try
  {

    if ( svm_save_model( fileName.c_str(), _model ) )
    {

      throw std::runtime_error(
            std::string( "unable to write model to '" ) + fileName + "' file" );

    }

  }
  GKG_CATCH( "void gkg::LibsvmSupportVectorMachineImplementation::write( "
             "const std::string& fileName ) const" );

}

void gkg::LibsvmSupportVectorMachineImplementation::read(
                                                   const std::string& fileName )
{

  try
  {

    _model = svm_load_model( fileName.c_str() );

  }
  GKG_CATCH( "void gkg::LibsvmSupportVectorMachineImplementation::read( "
             "const std::string& fileName )" );

}


void gkg::LibsvmSupportVectorMachineImplementation::svmPrintFunc( const char* )
{
}
