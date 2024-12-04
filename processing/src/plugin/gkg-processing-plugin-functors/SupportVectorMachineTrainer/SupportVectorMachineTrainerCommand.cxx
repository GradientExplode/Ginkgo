#include <gkg-processing-plugin-functors/SupportVectorMachineTrainer/SupportVectorMachineTrainerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-machinelearning/TrainScaler.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-processing-io/VectorItemIOFactory.h>
#include <gkg-core-io/StdVectorItemIOFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>



gkg::SupportVectorMachineTrainerCommand::SupportVectorMachineTrainerCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SupportVectorMachineTrainerCommand::"
             "SupportVectorMachineTrainerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SupportVectorMachineTrainerCommand::SupportVectorMachineTrainerCommand(
                                        const std::string& fileNameData,
                                        const std::string& fileNameLabels,
                                        const std::string& fileNameWeights,
                                        const std::string& fileNameOut,
                                        const std::string& svmTypeString,
                                        const std::string& kernelTypeString,
                                        int32_t degree,
                                        double gamma,
                                        double coef0,
                                        double cost,
                                        double nu,
                                        double epsilonSVR,
                                        double epsilonTermination,
                                        int32_t cacheSizeMB,
                                        bool shrinkingBoolean,
                                        bool verbose )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameData,
             fileNameLabels,
             fileNameWeights,
             fileNameOut,
             svmTypeString,
             kernelTypeString,
             degree,
             gamma,
             coef0,
             cost,
             nu,
             epsilonSVR,
             epsilonTermination,
             cacheSizeMB,
             shrinkingBoolean,
             verbose );

  }
  GKG_CATCH( "gkg::SupportVectorMachineTrainerCommand::"
             "SupportVectorMachineTrainerCommand( "
             "const std::string& fileNameData, "
             "const std::string& fileNameLabels, "
             "const std::string& fileNameWeights, "
             "const std::string& fileNameOut, "
             "const std::string& svmTypeString, "
             "const std::string& kernelTypeString, "
             "int32_t degree, "
             "double gamma, "
             "double coef0, "
             "double cost, "
             "double nu, "
             "double epsilonSVR, "
             "double epsilonTermination, "
             "int32_t cacheSizeMB, "
             "bool shrinkingBoolean, "
             "bool verbose )" );

}


gkg::SupportVectorMachineTrainerCommand::SupportVectorMachineTrainerCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameData );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameLabels );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameWeights );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, svmTypeString );
    DECLARE_STRING_PARAMETER( parameters, std::string, kernelTypeString );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, degree );
    DECLARE_FLOATING_PARAMETER( parameters, double, gamma );
    DECLARE_FLOATING_PARAMETER( parameters, double, coef0 );
    DECLARE_FLOATING_PARAMETER( parameters, double, cost );
    DECLARE_FLOATING_PARAMETER( parameters, double, nu );
    DECLARE_FLOATING_PARAMETER( parameters, double, epsilonSVR );
    DECLARE_FLOATING_PARAMETER( parameters, double, epsilonTermination );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, cacheSizeMB );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, shrinkingBoolean );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameData,
             fileNameLabels,
             fileNameWeights,
             fileNameOut,
             svmTypeString,
             kernelTypeString,
             degree,
             gamma,
             coef0,
             cost,
             nu,
             epsilonSVR,
             epsilonTermination,
             cacheSizeMB,
             shrinkingBoolean,
             verbose );

  }
  GKG_CATCH( "gkg::SupportVectorMachineTrainerCommand::"
             "SupportVectorMachineTrainerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SupportVectorMachineTrainerCommand::~SupportVectorMachineTrainerCommand()
{
}


std::string gkg::SupportVectorMachineTrainerCommand::getStaticName()
{

  try
  {

    return "SupportVectorMachineTrainer";

  }
  GKG_CATCH( "std::string "
             "gkg::SupportVectorMachineTrainerCommand::getStaticName()" );

}


void gkg::SupportVectorMachineTrainerCommand::parse()
{

  try
  {

    std::string fileNameData;
    std::string fileNameLabels;
    std::string fileNameWeights;
    std::string fileNameOut;
    std::string svmTypeString = "C_SVC";
    std::string kernelTypeString = "RadialBasisFunction";
    int32_t degree = 3;
    double gamma = 0.0;
    double coef0 = 0.0;
    double cost = 1.0;
    double nu = 0.5;
    double epsilonSVR = 0.1;
    double epsilonTermination = 0.001;
    int32_t cacheSizeMB = 100;
    bool shrinkingBoolean = true;
    bool verbose = false;


    gkg::Application application( _argc, _argv,
                                  "Support Vector Machine trainer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input data sparse matrix file name",
                                 fileNameData );
    application.addSingleOption( "-l",
                                 "Input labels vector file name",
                                 fileNameLabels );
    application.addSingleOption( "-w",
                                 "Input weights vector file name",
                                 fileNameWeights,
                                 true );
    application.addSingleOption( "-o",
                                 "Output SVM model file name",
                                 fileNameOut );
    application.addSingleOption( "-svmType",
                                 "SVM type, one of:\n"
                                 " - C_SVC -> C-SVM classifier\n"
                                 " - nu_SVC -> nu-SVM classifier\n"
                                 " - one_class_SVM -> one class SVM\n"
                                 " - epsilon_SVR -> epsilon-SVM regression\n"
                                 " - nu_SVR -> nu-SVM regression\n"
                                 "(default=C_SVC)\n",
                                 svmTypeString,
                                 true );
    application.addSingleOption( "-kernelType",
                                 "SVM type, one of:\n"
                                 " - Linear -> u'*v\n"
                                 " - Polynomial -> (gamma*u'*v+coef0)^degree\n"
                                 " - RadialBasisFunction -> "
                                 "exp(-gamma*(u'-v)^2)\n"
                                 " - Sigmoid -> tanh(gamma*u'*v+coef0)\n"
                                 "(default=RadialBasisFunction)\n",
                                 kernelTypeString,
                                 true );
    application.addSingleOption( "-degree",
                                 "Degree for polynom kernel type (default=3)",
                                 degree,
                                 true );
    application.addSingleOption( "-gamma",
                                 "gamma parameter (default=1/k)",
                                 gamma,
                                 true );
    application.addSingleOption( "-coef0",
                                 "coef0 parameter (default=0.0)",
                                 coef0,
                                 true );
    application.addSingleOption( "-cost",
                                 "Set the C parameter for "
                                 "C-SVC/epsilon-SVR/nu-SVR (default=1.0)",
                                 cost,
                                 true );
    application.addSingleOption( "-nu",
                                 "Set the nu parameter of "
                                 "nu-SVC/one_class_SVM/nu-SVR (default=0.5)",
                                 nu,
                                 true );
    application.addSingleOption( "-epsilonSVR",
                                 "Set the epsilon in loss function of "
                                 "epsilon-SVR (default=0.1)",
                                 nu,
                                 true );
    application.addSingleOption( "-epsilonTermination",
                                 "Set the tolerance of termination criterion"
                                 "(default=0.001)",
                                 epsilonTermination,
                                 true );
    application.addSingleOption( "-cacheSizeInMB",
                                 "Set the cache size in MBytes"
                                 "(default=100)",
                                 cacheSizeMB,
                                 true );
    application.addSingleOption( "-shrinking",
                                 "Using the shrinking heuristics"
                                 "(default=true)",
                                 shrinkingBoolean,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    // sanity checks()

    execute( fileNameData,
             fileNameLabels,
             fileNameWeights,
             fileNameOut,
             svmTypeString,
             kernelTypeString,
             degree,
             gamma,
             coef0,
             cost,
             nu,
             epsilonSVR,
             epsilonTermination,
             cacheSizeMB,
             shrinkingBoolean,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SupportVectorMachineTrainerCommand::parse()" );

}


void gkg::SupportVectorMachineTrainerCommand::execute(
                                      const std::string& fileNameData,
                                      const std::string& fileNameLabels,
                                      const std::string& fileNameWeights,
                                      const std::string& fileNameOut,
                        	      const std::string& svmTypeString,
                                      const std::string& kernelTypeString,
                                      int32_t degree,
                                      double gamma,
                                      double coef0,
                                      double cost,
                                      double nu,
                                      double epsilonSVR,
                                      double epsilonTermination,
                                      int32_t cacheSizeMB,
                                      bool shrinkingBoolean,
                        	      bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::SupportVectorMachine::SVMType
      svmType = gkg::SupportVectorMachine::C_SVC;
    if ( svmTypeString == "C_SVC" )
    {

      svmType = gkg::SupportVectorMachine::C_SVC;

    }
    else if ( svmTypeString == "nu_SVC" )
    {

      svmType = gkg::SupportVectorMachine::nu_SVC;

    }
    else if ( svmTypeString == "one_class_SVM" )
    {

      svmType = gkg::SupportVectorMachine::one_class_SVM;

    }
    else if ( svmTypeString == "epsilon_SVR" )
    {

      svmType = gkg::SupportVectorMachine::epsilon_SVR;

    }
    else if ( svmTypeString == "nu_SVR" )
    {

      svmType = gkg::SupportVectorMachine::nu_SVR;

    }
    else
    {

      throw std::runtime_error( "bad SVM type" );

    }

    gkg::SupportVectorMachine::KernelType
      kernelType = gkg::SupportVectorMachine::Linear;
    if ( kernelTypeString == "Linear" )
    {

      kernelType = gkg::SupportVectorMachine::Linear;

    }
    else if ( kernelTypeString == "Polynomial" )
    {

      kernelType = gkg::SupportVectorMachine::Polynomial;

    }
    else if ( kernelTypeString == "RadialBasisFunction" )
    {

      kernelType = gkg::SupportVectorMachine::RadialBasisFunction;

    }
    else if ( kernelTypeString == "Sigmoid" )
    {

      kernelType = gkg::SupportVectorMachine::Sigmoid;

    }
    else
    {

      throw std::runtime_error( "bad kernel type" );

    }

    int32_t shrinking = ( shrinkingBoolean ? 1 : 0 );

    ////////////////////////////////////////////////////////////////////////////
    // reading input data sparse matrix
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "reading input data sparse matrix : " << std::flush;

    }

    gkg::ItemReader< gkg::SparseMatrix >& sparseMatrixR =
      gkg::SparseMatrixItemIOFactory::getInstance().getReader( true, false );

    gkg::SparseMatrix trainingData;

    std::ifstream is( fileNameData.c_str() );
    sparseMatrixR.read( is, trainingData );
    is.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // reading input labels
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "reading input labels : " << std::flush;

    }

    gkg::ItemReader< gkg::Vector >& vectorR =
      gkg::VectorItemIOFactory::getInstance().getReader( true, false );

    gkg::Vector trainingLabels;

    is.open( fileNameLabels.c_str() );
    vectorR.read( is, trainingLabels );
    is.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // reading input weights
    ////////////////////////////////////////////////////////////////////////////
    std::vector< double > weights;
    std::vector< int32_t > indices;
    if ( !fileNameWeights.empty() )
    {

      if ( verbose )
      {

       std::cout << "reading input weights : " << std::flush;

      }

      gkg::ItemReader< std::vector< double > >& stdVectorR =
        gkg::StdVectorItemIOFactory< double >::getInstance().getReader( true,
                                                                        false );

      is.open( fileNameWeights.c_str() );
      stdVectorR.read( is, weights );
      is.close();

      indices.resize( weights.size() );

      int32_t index = 1;
      std::vector< int32_t >::iterator
        i = indices.begin(),
        ie = indices.end();
      while ( i != ie )
      {

        *i = index;
        ++ i;
        ++ index;

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // creating support vector machine
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "creating support vector machine : " << std::flush;

    }

    gkg::SupportVectorMachine
      supportVectorMachine( svmType,
                            kernelType,
                            degree,
                            gamma,
                            coef0,
                            cost,
                            nu,
                            epsilonSVR,
                            epsilonTermination,
                            cacheSizeMB,
                            shrinking );

    if ( !fileNameWeights.empty() )
    {

      supportVectorMachine.setWeights( indices, weights );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // training support vector machine
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "training support vector machine : " << std::flush;

    }

    gkg::TrainScaler trainScaler;
    gkg::RCPointer< gkg::MachineLearningData > machineLearningData( 
                    new gkg::MachineLearningData( trainingData, trainScaler ) );
    gkg::MachineLearningProblem machineLearningProblem( machineLearningData,
                                                        trainingLabels );

    supportVectorMachine.train( machineLearningProblem );


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing support vector machine
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "writing support vector machine : " << std::flush;

    }

    supportVectorMachine.write( fileNameOut );

    // writing the scales
    std::string scaleFileName = fileNameOut + ".scale";
    gkg::ItemWriter< gkg::Matrix >& matrixW =
      gkg::MatrixItemIOFactory::getInstance().getWriter( true, false );

    std::ofstream os( scaleFileName.c_str() );
    matrixW.write( os, trainScaler.getScaleMatrix() );
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SupportVectorMachineTrainerCommand::execute( "
             "const std::string& fileNameData, "
             "const std::string& fileNameLabels, "
             "const std::string& fileNameWeights, "
             "const std::string& fileNameOut, "
             "const std::string& svmTypeString, "
             "const std::string& kernelTypeString, "
             "int32_t degree, "
             "double gamma, "
             "double coef0, "
             "double cost, "
             "double nu, "
             "double epsilonSVR, "
             "double epsilonTermination, "
             "int32_t cacheSizeMB, "
             "bool shrinkingBoolean, "
             "bool verbose )" );

}


RegisterCommandCreator( SupportVectorMachineTrainerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameData ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameLabels ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameWeights ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( svmTypeString ) +
    			DECLARE_STRING_PARAMETER_HELP( kernelTypeString ) +
    			DECLARE_INTEGER_PARAMETER_HELP( degree ) +
    			DECLARE_FLOATING_PARAMETER_HELP( gamma ) +
    			DECLARE_FLOATING_PARAMETER_HELP( coef0 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( cost ) +
    			DECLARE_FLOATING_PARAMETER_HELP( nu ) +
    			DECLARE_FLOATING_PARAMETER_HELP( epsilonSVR ) +
    			DECLARE_FLOATING_PARAMETER_HELP( epsilonTermination ) +
    			DECLARE_INTEGER_PARAMETER_HELP( cacheSizeMB ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( shrinkingBoolean ) );
