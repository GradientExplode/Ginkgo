#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::PowellNewUOAFunction::PowellNewUOAFunction( int32_t parameterCount )
                          : _parameterCount( parameterCount )
{
}


gkg::PowellNewUOAFunction::~PowellNewUOAFunction()
{
}


int32_t gkg::PowellNewUOAFunction::getParameterCount() const
{

  return _parameterCount;

}


void gkg::PowellNewUOAFunction::addConstraints(
                const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >&
                                                                   constraints )
{

  try
  {

    // sanity checks
    if ( ( int32_t )constraints.size() != _parameterCount )
    {

      throw std::runtime_error( 
         "the size of vector of constraints do not match the parameter count" );

    }
    int32_t p = 0;
    for ( p = 0; p < _parameterCount; p++ )
    {

      if ( constraints[ p ].isNull() )
      {

        throw std::runtime_error( 
                                "some of the constraints are nul RC pointers" );

      }

    }

    // making a copy of the constraints
    _constraints = constraints;

  }
  GKG_CATCH( "void gkg::PowellNewUOAFunction::addConstraints( "
             "const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& "
             "constraints )" );


}


void gkg::PowellNewUOAFunction::getConstrainedParameters(
                                     const gkg::Vector& unconstrainedParameters,
                                     gkg::Vector& constrainedParameters ) const
{

  try
  {

    // sanity check
    if ( constrainedParameters.getSize() !=
         unconstrainedParameters.getSize() )
    {

      throw std::runtime_error(
                          "vectors of unconstrained and constrained parameters "
                          "have inconsistent size(s)" );

    }

    this->applyConstraints( &unconstrainedParameters( 0 ),
                            unconstrainedParameters.getSize(),
                            &constrainedParameters( 0 ) );

  }
  GKG_CATCH( "void gkg::PowellNewUOAFunction::getConstrainedParameters( "
             "const gkg::Vector& unconstrainedParameters, "
             "gkg::Vector& constrainedParameters ) const" );

}


double gkg::PowellNewUOAFunction::getValueAt( const Vector& parameters ) const
{

  try
  {

    return this->getValueAt( &parameters( 0 ), parameters.getSize() );

  }
  GKG_CATCH( "double gkg::PowellNewUOAFunction::getValueAt( "
             "const Vector& parameters ) const" );

}


void gkg::PowellNewUOAFunction::applyConstraints(
                     const double* unconstrainedParameters, int32_t /* count */,
                     double* constrainedParameters ) const
{

  try
  {

    int32_t p = 0;
    for ( p = 0; p < _parameterCount; p++ )
    {

      constrainedParameters[ p ] = _constraints[ p ]->inverseTransform(
                                                 unconstrainedParameters[ p ] );

    }

  }
  GKG_CATCH( "void gkg::PowellNewUOAFunction::applyConstraints( "
             "const double* unconstrainedParameters, int32_t count, "
             "double* constrainedParameters ) const" );

}


