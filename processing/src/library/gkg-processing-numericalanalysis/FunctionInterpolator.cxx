#include <gkg-processing-numericalanalysis/FunctionInterpolator.h>
#include <gkg-processing-numericalanalysis/FunctionInterpolatorImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::FunctionInterpolator::FunctionInterpolator( 
                                          gkg::FunctionInterpolator::Type type,
                                          const std::vector< double >& xs,
                                          const std::vector< double >& ys )
{

  try
  {

   _functionInterpolatorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                         createFunctionInterpolatorImplementation( this,
                                                                   type,
                                                                   xs,
                                                                   ys );

  }
  GKG_CATCH( "gkg::FunctionInterpolator::FunctionInterpolator( "
             "gkg::FunctionInterpolator::Type type, "
             "const std::vector< double >& xs, "
             "const std::vector< double >& ys )" );

}


gkg::FunctionInterpolator::FunctionInterpolator( 
                                          gkg::FunctionInterpolator::Type type,
                                          const gkg::Vector& xs,
                                          const gkg::Vector& ys )
{

  try
  {

   _functionInterpolatorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                         createFunctionInterpolatorImplementation( this,
                                                                   type,
                                                                   xs,
                                                                   ys );

  }
  GKG_CATCH( "gkg::FunctionInterpolator::FunctionInterpolator( "
             "gkg::FunctionInterpolator::Type type, "
             "const gkg::Vector& xs, "
             "const gkg::Vector& ys )" );

}


gkg::FunctionInterpolator::~FunctionInterpolator()
{

  delete _functionInterpolatorImplementation;

}


double gkg::FunctionInterpolator::getValueAt( double x ) const
{

  try
  {

    return _functionInterpolatorImplementation->getValueAt( x );

  }
  GKG_CATCH( "double gkg::FunctionInterpolator::getValueAt( double x ) const" );

}



gkg::FunctionInterpolatorImplementation* 
gkg::FunctionInterpolator::getImplementation() const
{

  return _functionInterpolatorImplementation;

}
