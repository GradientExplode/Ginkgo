#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>



////////////////////////////////////////////////////////////////////////////////
// class MyNLPFunction
////////////////////////////////////////////////////////////////////////////////

class MyNLPFunction : public gkg::NonLinearProgrammingFunction
{

  public:

    MyNLPFunction();
    virtual ~MyNLPFunction();

    double getValueAt( const gkg::Vector& parameters ) const;
    
    void getDerivativeAt( const gkg::Vector& parameters,
                          gkg::Vector& derivative ) const;
    double getValueAndDerivativeAt( const gkg::Vector& parameters,
                                    gkg::Vector& derivative ) const;

};


MyNLPFunction::MyNLPFunction()
              : gkg::NonLinearProgrammingFunction( 2, true )
{
}


MyNLPFunction::~MyNLPFunction()
{
}


double MyNLPFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    return std::sqrt( parameters( 1 ) );

  }
  GKG_CATCH( "double MyNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


void MyNLPFunction::getDerivativeAt( const gkg::Vector& parameters,
                                     gkg::Vector& derivative ) const

{

  try
  {

    derivative( 0 ) = 0.0;
    derivative( 1 ) = 0.5 / std::sqrt( parameters( 1 ) ); 


  }
  GKG_CATCH( "void MyNLPFunction::getDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


double MyNLPFunction::getValueAndDerivativeAt( const gkg::Vector& parameters,
                                               gkg::Vector& derivative ) const
{

  try
  {

    this->getDerivativeAt( parameters, derivative );
    return this->getValueAt( parameters );

  }
  GKG_CATCH( "double MyNLPFunction::getValueAndDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// class MyFirstConstraint
////////////////////////////////////////////////////////////////////////////////

class MyFirstConstraint : public gkg::NonLinearProgrammingFunction
{

  public:

    MyFirstConstraint();
    virtual ~MyFirstConstraint();

    double getValueAt( const gkg::Vector& parameters ) const;
    
    void getDerivativeAt( const gkg::Vector& parameters,
                          gkg::Vector& derivative ) const;
    double getValueAndDerivativeAt( const gkg::Vector& parameters,
                                    gkg::Vector& derivative ) const;

};


MyFirstConstraint::MyFirstConstraint()
                  : gkg::NonLinearProgrammingFunction( 2, true )
{
}


MyFirstConstraint::~MyFirstConstraint()
{
}


double MyFirstConstraint::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    return ( 2.0 * parameters( 0 ) ) *
           ( 2.0 * parameters( 0 ) ) *
           ( 2.0 * parameters( 0 ) )
           - parameters( 1 );

  }
  GKG_CATCH( "double MyFirstConstraint::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


void MyFirstConstraint::getDerivativeAt( const gkg::Vector& parameters,
                                         gkg::Vector& derivative ) const

{

  try
  {

    derivative( 0 ) = 3 * 2.0 *
                      ( 2.0 * parameters( 0 ) ) * ( 2.0 * parameters( 0 ) );
    derivative( 1 ) = -1.0; 


  }
  GKG_CATCH( "void MyFirstConstraint::getDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


double MyFirstConstraint::getValueAndDerivativeAt(
                                                 const gkg::Vector& parameters,
                                                 gkg::Vector& derivative ) const
{

  try
  {

    this->getDerivativeAt( parameters, derivative );
    return this->getValueAt( parameters );

  }
  GKG_CATCH( "double MyFirstConstraint::getValueAndDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// class MySecondConstraint
////////////////////////////////////////////////////////////////////////////////

class MySecondConstraint : public gkg::NonLinearProgrammingFunction
{

  public:

    MySecondConstraint();
    virtual ~MySecondConstraint();

    double getValueAt( const gkg::Vector& parameters ) const;
    
    void getDerivativeAt( const gkg::Vector& parameters,
                          gkg::Vector& derivative ) const;
    double getValueAndDerivativeAt( const gkg::Vector& parameters,
                                    gkg::Vector& derivative ) const;

};


MySecondConstraint::MySecondConstraint()
                   : gkg::NonLinearProgrammingFunction( 2, true )
{
}


MySecondConstraint::~MySecondConstraint()
{
}


double MySecondConstraint::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    return ( -1.0 * parameters( 0 ) + 1.0 ) *
           ( -1.0 * parameters( 0 ) + 1.0 ) *
           ( -1.0 * parameters( 0 ) + 1.0 )
           - parameters( 1 );

  }
  GKG_CATCH( "double MySecondConstraint::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


void MySecondConstraint::getDerivativeAt( const gkg::Vector& parameters,
                                          gkg::Vector& derivative ) const

{

  try
  {

    derivative( 0 ) = 3 * ( -1.0 ) *
                      ( ( -1.0 ) * parameters( 0 ) + 1.0 ) *
                      ( ( -1.0 ) * parameters( 0 ) + 1.0 );
    derivative( 1 ) = -1.0; 


  }
  GKG_CATCH( "void MySecondConstraint::getDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


double MySecondConstraint::getValueAndDerivativeAt(
                                                 const gkg::Vector& parameters,
                                                 gkg::Vector& derivative ) const
{

  try
  {

    this->getDerivativeAt( parameters, derivative );
    return this->getValueAt( parameters );

  }
  GKG_CATCH( "double MySecondConstraint::getValueAndDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {


    gkg::Application application( argc, argv,
                                  "NLP solver second testing" );
    application.initialize();


    std::vector< gkg::RCPointer< gkg::NonLinearProgrammingFunction > >
      inequalityConstraints( 2 );
    std::vector< gkg::RCPointer< gkg::NonLinearProgrammingFunction > >
      equalityConstraints;

    inequalityConstraints[ 0 ].reset( new MyFirstConstraint );
    inequalityConstraints[ 1 ].reset( new MySecondConstraint );

    gkg::Vector initialParameters( 2 );
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    std::vector< double > parameterLowerBounds( 2 );
    parameterLowerBounds[ 0 ] = -HUGE_VAL;
    parameterLowerBounds[ 1 ] = 0.0;

    std::vector< double > parameterUpperBounds( 2 );
    parameterUpperBounds[ 0 ] = HUGE_VAL;
    parameterUpperBounds[ 1 ] = HUGE_VAL;

    std::vector< double > inequalityTolerances( 2 );
    inequalityTolerances[ 0 ] = 1e-8;
    inequalityTolerances[ 1 ] = 1e-8;

    std::vector< double > equalityTolerances;

    double relativeTolerance = 1e-4;

    MyNLPFunction myNLPFunction;

    int32_t achievedIterationCount = 0;
    double reachedMinimumObjectiveValue = 0.0f;


    std::cout << "================ TEST WO DERIVATIVES ==============="
              << std::endl;

    initialParameters( 0 ) = 1.234;
    initialParameters( 1 ) = 5.678;


    factory->getConstrainedNonLinearProgrammingOptimumParameters2(
          gkg::NumericalAnalysisImplementationFactory::LocalWoDerivative_COBYLA,
          initialParameters,
          inequalityConstraints,
          equalityConstraints,
          myNLPFunction,
          parameterLowerBounds,
          parameterUpperBounds,
          inequalityTolerances,
          equalityTolerances,
          relativeTolerance,
          &achievedIterationCount,
          &reachedMinimumObjectiveValue,
          true );

    std::cout << "----------------------------------------------------"
              << std::endl;
    std::cout << "expected parameters: 0.333334 0.296296"
              << std::endl
              << "(minimum objective value : 0.544330847)"
              << std::endl;

    std::cout << "================ TEST WITH DERIVATIVES ============="
              << std::endl;

    initialParameters( 0 ) = 1.234;
    initialParameters( 1 ) = 5.678;


    factory->getConstrainedNonLinearProgrammingOptimumParameters2(
          gkg::NumericalAnalysisImplementationFactory::LocalWithDerivative_MMA,
          initialParameters,
          inequalityConstraints,
          equalityConstraints,
          myNLPFunction,
          parameterLowerBounds,
          parameterUpperBounds,
          inequalityTolerances,
          equalityTolerances,
          relativeTolerance,
          &achievedIterationCount,
          &reachedMinimumObjectiveValue,
          true );

    std::cout << "----------------------------------------------------"
              << std::endl;
    std::cout << "expected parameters: 0.333334 0.296296"
              << std::endl
              << "(minimum objective value : 0.544330847)"
              << std::endl;



  }
  GKG_CATCH_COMMAND( result );

  return result;


}
