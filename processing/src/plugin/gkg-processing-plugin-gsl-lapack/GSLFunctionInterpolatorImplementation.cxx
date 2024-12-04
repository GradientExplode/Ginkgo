#include <gkg-processing-plugin-gsl-lapack/GSLFunctionInterpolatorImplementation.h>
#include <gkg-processing-numericalanalysis/FunctionInterpolator.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>

gkg::GSLFunctionInterpolatorImplementation::GSLFunctionInterpolatorImplementation(
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type type,
                                const std::vector< double >& xs,
                                const std::vector< double >& ys )
                                      : gkg::FunctionInterpolatorImplementation(
                                                           functionInterpolator,
                                                           type,
                                                           xs,
                                                           ys )
{

  try
  {

    // sanity check
    if ( xs.size() != ys.size() )
    {

      throw std::runtime_error( "xs and ys have different size(s)" );

    }

    _accelerator = gsl_interp_accel_alloc();

    if ( type == gkg::FunctionInterpolator::Linear )
    {

      _spline = gsl_spline_alloc( gsl_interp_linear, xs.size() );

    }
    else if ( type == gkg::FunctionInterpolator::Polynomial )
    {

      _spline = gsl_spline_alloc( gsl_interp_polynomial, xs.size() );

    }
    else if ( type == gkg::FunctionInterpolator::CubicSpline )
    {

      _spline = gsl_spline_alloc( gsl_interp_cspline, xs.size() );

    }
    else if ( type == gkg::FunctionInterpolator::CubicSplinePeriodic )
    {

      _spline = gsl_spline_alloc( gsl_interp_cspline_periodic, xs.size() );

    }
    else if ( type == gkg::FunctionInterpolator::NonRoundedAkimaSpline )
    {

      _spline = gsl_spline_alloc( gsl_interp_akima, xs.size() );

    }
    else if ( type == gkg::FunctionInterpolator::NonRoundedAkimaSplinePeriodic )
    {

      _spline = gsl_spline_alloc( gsl_interp_akima_periodic, xs.size() );

    }

    gsl_spline_init( _spline, &xs[ 0 ], &ys[ 0 ], xs.size() );

  }
  GKG_CATCH( "gkg::GSLFunctionInterpolatorImplementation::"
             "GSLFunctionInterpolatorImplementation( "
             "gkg::FunctionInterpolator* functionInterpolator, "
             "gkg::FunctionInterpolator::Type type," 
             "const std::vector< double >& xs, "
             "const std::vector< double >& ys )" );

}


gkg::GSLFunctionInterpolatorImplementation::
                                          GSLFunctionInterpolatorImplementation(
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type type,
                                const gkg::Vector& xs,
                                const gkg::Vector& ys )
                                      : gkg::FunctionInterpolatorImplementation(
                                                           functionInterpolator,
                                                           type,
                                                           xs,
                                                           ys )
{

  try
  {

    // sanity check
    if ( xs.getSize() != ys.getSize() )
    {

      throw std::runtime_error( "xs and ys have different size(s)" );

    }

    _accelerator = gsl_interp_accel_alloc();

    if ( type == gkg::FunctionInterpolator::Linear )
    {

      _spline = gsl_spline_alloc( gsl_interp_linear, xs.getSize() );

    }
    else if ( type == gkg::FunctionInterpolator::Polynomial )
    {

      _spline = gsl_spline_alloc( gsl_interp_polynomial, xs.getSize() );

    }
    else if ( type == gkg::FunctionInterpolator::CubicSpline )
    {

      _spline = gsl_spline_alloc( gsl_interp_cspline, xs.getSize() );

    }
    else if ( type == gkg::FunctionInterpolator::CubicSplinePeriodic )
    {

      _spline = gsl_spline_alloc( gsl_interp_cspline_periodic, xs.getSize() );

    }
    else if ( type == gkg::FunctionInterpolator::NonRoundedAkimaSpline )
    {

      _spline = gsl_spline_alloc( gsl_interp_akima, xs.getSize() );

    }
    else if ( type == gkg::FunctionInterpolator::NonRoundedAkimaSplinePeriodic )
    {

      _spline = gsl_spline_alloc( gsl_interp_akima_periodic, xs.getSize() );

    }

    gsl_spline_init( _spline, &xs( 0 ), &ys( 0 ), ( size_t )xs.getSize() );

  }
  GKG_CATCH( "gkg::GSLFunctionInterpolatorImplementation::"
             "GSLFunctionInterpolatorImplementation( "
             "gkg::FunctionInterpolator* functionInterpolator, "
             "gkg::FunctionInterpolator::Type type," 
             "const gkg::Vector& xs, "
             "const gkg::Vector& ys )" );

}


gkg::GSLFunctionInterpolatorImplementation::
                                        ~GSLFunctionInterpolatorImplementation()
{

  gsl_spline_free( _spline );
  gsl_interp_accel_free( _accelerator );

}


double 
gkg::GSLFunctionInterpolatorImplementation::getValueAt( double x ) const
{

  try
  {

    return gsl_spline_eval( _spline, x, _accelerator );

  }
  GKG_CATCH( "double "
             "gkg::GSLFunctionInterpolatorImplementation::getValueAt( "
             "double x ) const" );

}


gkg::GSLFunctionInterpolatorImplementation* 
gkg::GSLFunctionInterpolatorImplementation::getImplementation(
                               gkg::FunctionInterpolator* functionInterpolator )
{

  return static_cast< gkg::GSLFunctionInterpolatorImplementation* >(
                                    functionInterpolator->getImplementation() );

}

