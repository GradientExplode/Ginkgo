#include <gkg-processing-plugin-functors/SupportVectorMachinePredicter/SupportVectorMachinePredicterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-machinelearning/PredictScaler.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-io/VectorItemIOFactory.h>
#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>



gkg::SupportVectorMachinePredicterCommand::SupportVectorMachinePredicterCommand(
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
  GKG_CATCH( "gkg::SupportVectorMachinePredicterCommand::"
             "SupportVectorMachinePredicterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SupportVectorMachinePredicterCommand::SupportVectorMachinePredicterCommand(
                                       const std::string& fileNameData,
                                       const std::string& fileNameModel,
                                       const std::string& fileNameProbabilities,
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
             fileNameModel,
             fileNameProbabilities,
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
  GKG_CATCH( "gkg::SupportVectorMachinePredicterCommand::"
             "SupportVectorMachinePredicterCommand( "
             "const std::string& fileNameData, "
             "const std::string& fileNameModel, "
             "const std::string& fileNameProbabilities, "
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


gkg::SupportVectorMachinePredicterCommand::SupportVectorMachinePredicterCommand(
                                             const gkg::Dictionary& parameters )
                                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameData );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameModel );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameProbabilities );
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
             fileNameModel,
             fileNameProbabilities,
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
  GKG_CATCH( "gkg::SupportVectorMachinePredicterCommand::"
             "SupportVectorMachinePredicterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SupportVectorMachinePredicterCommand::
                                         ~SupportVectorMachinePredicterCommand()
{
}


std::string gkg::SupportVectorMachinePredicterCommand::getStaticName()
{

  try
  {

    return "SupportVectorMachinePredicter";

  }
  GKG_CATCH( "std::string "
             "gkg::SupportVectorMachinePredicterCommand::getStaticName()" );

}


void gkg::SupportVectorMachinePredicterCommand::parse()
{

  try
  {

    std::string fileNameData;
    std::string fileNameModel;
    std::string fileNameProbabilities;
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
                                  "Support Vector Machine predicter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input data sparse matrix file name",
                                 fileNameData );
    application.addSingleOption( "-m",
                                 "Input SVM model file name",
                                 fileNameModel );
    application.addSingleOption( "-p",
                                 "Output probability matrix file name",
                                 fileNameProbabilities,
                                 true );
    application.addSingleOption( "-o",
                                 "Output prediction vector file name",
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
             fileNameModel,
             fileNameProbabilities,
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
  GKG_CATCH_FUNCTOR( "void gkg::SupportVectorMachinePredicterCommand::parse()" );

}


void gkg::SupportVectorMachinePredicterCommand::execute(
                                      const std::string& fileNameData,
                                      const std::string& fileNameModel,
                                      const std::string& fileNameProbabilities,
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

    gkg::SparseMatrix testData;

    std::ifstream is( fileNameData.c_str() );
    sparseMatrixR.read( is, testData );
    is.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading support vector machine
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "reading support vector machine : " << std::flush;

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
    supportVectorMachine.read( fileNameModel );

    // reading the scales
    gkg::PredictScaler predictScaler;
    std::string fileNameScale = fileNameModel + ".scale";
    gkg::ItemReader< gkg::Matrix >& matrixR =
      gkg::MatrixItemIOFactory::getInstance().getReader( true, false );

    is.open( fileNameScale.c_str() );

    if ( is.good() )
    {

      gkg::Matrix scales;

      matrixR.read( is, scales );
      is.close();

      int32_t size2 = scales.getSize2() - 1;

      double outMin = scales( 0, size2 + 1 );
      double outMax = scales( 1, size2 + 1 );
      gkg::Matrix inMinMax( 2, size2 );
      int32_t s;

      for ( s = 0; s < size2; s++ )
      {

        inMinMax( 0, s ) = scales( 0, s );
        inMinMax( 1, s ) = scales( 1, s );

      }

      predictScaler = gkg::PredictScaler( inMinMax, outMin, outMax );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // predicting labels with support vector machine
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "predicting labels using support vector machine : "
               << std::flush;

    }

    gkg::MachineLearningData machineLearningData( testData, predictScaler );
    gkg::Vector testLabels;
    gkg::Matrix testProbabilities;

    if ( fileNameProbabilities.empty() )
    {

      supportVectorMachine.predict( machineLearningData,
                                    testLabels );

    }
    else
    {

      supportVectorMachine.predict( machineLearningData,
                                    testLabels,
                                    &testProbabilities );

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output labels
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "writing output labels : " << std::flush;

    }

    gkg::ItemWriter< gkg::Vector >& vectorW =
      gkg::VectorItemIOFactory::getInstance().getWriter( true, false );

    std::ofstream os( fileNameOut.c_str() );
    vectorW.write( os, testLabels );
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // writing output probabilities
    ////////////////////////////////////////////////////////////////////////////
    if ( !fileNameProbabilities.empty() )
    {

      if ( verbose )
      {

       std::cout << "writing output probabilities : " << std::flush;

      }

      gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
      gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );

      os.open( fileNameProbabilities.c_str() );
      sparseMatrixW.write( os, gkg::SparseMatrix( testProbabilities ) );
      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::SupportVectorMachinePredicterCommand::execute( "
             "const std::string& fileNameData, "
             "const std::string& fileNameModel, "
             "const std::string& fileNameProbabilities, "
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


RegisterCommandCreator( SupportVectorMachinePredicterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameData ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameModel ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameProbabilities ) +
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
