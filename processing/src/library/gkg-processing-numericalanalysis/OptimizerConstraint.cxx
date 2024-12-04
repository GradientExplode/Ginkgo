#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


//
// class OptimizerConstraint
//

gkg::OptimizerConstraint::OptimizerConstraint()
{
}


gkg::OptimizerConstraint::~OptimizerConstraint()
{
}


double gkg::OptimizerConstraint::constrainedDeltaParameter(
                                                   double deltaParameter ) const
{

  try
  {

    return deltaParameter;

  }
  GKG_CATCH( "double gkg::OptimizerConstraint::constrainedDeltaParameter( "
             "double deltaParameter ) const" );


}


std::string 
gkg::OptimizerConstraint::displayTransformed( double value ) const
{

  try
  {

    return gkg::StringConverter::toString( value, 'g', 10, 2 ) +
           std::string( " " ) +
           this->displayType() +
           std::string( " -> " ) +
           gkg::StringConverter::toString(
                                         this->transform( value ), 'g', 10, 2 );

  }
  GKG_CATCH( "std::string "
             "gkg::OptimizerConstraint::displayTransformed( "
             "double value ) const" );

}


//
// class NoneOptimizerConstraint
//

gkg::NoneOptimizerConstraint::NoneOptimizerConstraint()
                             : gkg::OptimizerConstraint()
{
}


gkg::NoneOptimizerConstraint::~NoneOptimizerConstraint()
{
}


double gkg::NoneOptimizerConstraint::transform( double value ) const
{

  try
  {

    return value;

  }
  GKG_CATCH( "double gkg::NoneOptimizerConstraint::transform( "
             "double value ) const" );

}


double gkg::NoneOptimizerConstraint::inverseTransform( double value ) const
{

  try
  {

    return value;

  }
  GKG_CATCH( "double gkg::NoneOptimizerConstraint::inverseTransform( "
             "double value ) const" );

}


double gkg::NoneOptimizerConstraint::inverseDerivative( 
                                                      double /* value */ ) const
{

  try
  {

    return 1;

  }
  GKG_CATCH( "double gkg::NoneOptimizerConstraint::inverseDerivative( "
             "double value ) const" );

}


std::string gkg::NoneOptimizerConstraint::displayType() const
{

  try
  {

    return "(-inf,+inf)";

  }
  GKG_CATCH( "std::string gkg::NoneOptimizerConstraint::displayType() const" );

}


//
// class SemiClosedRightHalfLineOptimizerConstraint
//

gkg::SemiClosedRightHalfLineOptimizerConstraint::
                                    SemiClosedRightHalfLineOptimizerConstraint(
                                                                double minimum )
                                                : gkg::OptimizerConstraint(),
                                                  _minimum( minimum )
{
}


gkg::SemiClosedRightHalfLineOptimizerConstraint::
                                   ~SemiClosedRightHalfLineOptimizerConstraint()
{
}


double gkg::SemiClosedRightHalfLineOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    return std::sqrt( value - _minimum );

  }
  GKG_CATCH( "double gkg::SemiClosedRightHalfLineOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::SemiClosedRightHalfLineOptimizerConstraint::inverseTransform( 
                                                            double value ) const
{

  try
  {

    return _minimum + value * value;

  }
  GKG_CATCH( "double gkg::SemiClosedRightHalfLineOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::SemiClosedRightHalfLineOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    return 2 * value;

  }
  GKG_CATCH( "double "
             "gkg::SemiClosedRightHalfLineOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string gkg::SemiClosedRightHalfLineOptimizerConstraint::displayType() const
{

  try
  {

    return "[" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) +
           ",+inf)";

  }
  GKG_CATCH( "std::string "
             "gkg::SemiClosedRightHalfLineOptimizerConstraint::displayType() "
             "const" );

}


//
// class OpenRightHalfLineOptimizerConstraint
//

gkg::OpenRightHalfLineOptimizerConstraint::
                                          OpenRightHalfLineOptimizerConstraint(
                                                                double minimum )
                                                : gkg::OptimizerConstraint(),
                                                  _minimum( minimum )
{
}


gkg::OpenRightHalfLineOptimizerConstraint::
                                         ~OpenRightHalfLineOptimizerConstraint()
{
}


double gkg::OpenRightHalfLineOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    return std::log( value - _minimum );

  }
  GKG_CATCH( "double gkg::OpenRightHalfLineOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenRightHalfLineOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    return _minimum + std::exp( value );

  }
  GKG_CATCH( "double gkg::OpenRightHalfLineOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenRightHalfLineOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    return std::exp( value );

  }
  GKG_CATCH( "double "
             "gkg::OpenRightHalfLineOptimizerConstraint::inverseDerivative( "
             "double value ) const" );

}


std::string gkg::OpenRightHalfLineOptimizerConstraint::displayType() const
{

  try
  {

    return "(" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) +
           ",+inf)";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenRightHalfLineOptimizerConstraint::displayType() const" );

}


//
// class OpenRightHalfLineAlternativeOptimizerConstraint
//

gkg::OpenRightHalfLineAlternativeOptimizerConstraint::
                                OpenRightHalfLineAlternativeOptimizerConstraint(
                                                                double minimum )
                                                : gkg::OptimizerConstraint(),
                                                  _minimum( minimum )
{
}


gkg::OpenRightHalfLineAlternativeOptimizerConstraint::
                              ~OpenRightHalfLineAlternativeOptimizerConstraint()
{
}


double gkg::OpenRightHalfLineAlternativeOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    double tmp = value - _minimum;
    return tmp - 1.0 / ( 4.0 * tmp );

  }
  GKG_CATCH( "double gkg::OpenRightHalfLineAlternativeOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenRightHalfLineAlternativeOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    double tmp = std::sqrt( 1.0 + value * value );
    return _minimum + 0.5 * ( tmp + value );

  }
  GKG_CATCH( "double gkg::OpenRightHalfLineAlternativeOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenRightHalfLineAlternativeOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    double tmp = std::sqrt( 1.0 + value * value );
    return 0.5 * ( value + tmp ) / tmp;

  }
  GKG_CATCH( "double "
             "gkg::OpenRightHalfLineAlternativeOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string 
gkg::OpenRightHalfLineAlternativeOptimizerConstraint::displayType() const
{

  try
  {

    return "(" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) +
           ",+inf)";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenRightHalfLineAlternativeOptimizerConstraint::"
             "displayType() const" );

}


//
// class SemiClosedLeftHalfLineOptimizerConstraint
//

gkg::SemiClosedLeftHalfLineOptimizerConstraint::
                                     SemiClosedLeftHalfLineOptimizerConstraint(
                                                                double maximum )
                                                : gkg::OptimizerConstraint(),
                                                  _maximum( maximum )
{
}


gkg::SemiClosedLeftHalfLineOptimizerConstraint::
                                    ~SemiClosedLeftHalfLineOptimizerConstraint()
{
}


double gkg::SemiClosedLeftHalfLineOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    return std::sqrt( _maximum - value );

  }
  GKG_CATCH( "double gkg::SemiClosedLeftHalfLineOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::SemiClosedLeftHalfLineOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    return _maximum - value * value;

  }
  GKG_CATCH( "double gkg::SemiClosedLeftHalfLineOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::SemiClosedLeftHalfLineOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    return -2.0 * value;

  }
  GKG_CATCH( "double "
             "gkg::SemiClosedLeftHalfLineOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string gkg::SemiClosedLeftHalfLineOptimizerConstraint::displayType() const
{

  try
  {

    return "(-inf," + gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) +
           "]";

  }
  GKG_CATCH( "std::string "
             "gkg::SemiClosedLeftHalfLineOptimizerConstraint::displayType() "
             "const" );

}


//
// class OpenLeftHalfLineOptimizerConstraint
//

gkg::OpenLeftHalfLineOptimizerConstraint::
                                           OpenLeftHalfLineOptimizerConstraint(
                                                                double maximum )
                                                : gkg::OptimizerConstraint(),
                                                  _maximum( maximum )
{
}


gkg::OpenLeftHalfLineOptimizerConstraint::~OpenLeftHalfLineOptimizerConstraint()
{
}


double gkg::OpenLeftHalfLineOptimizerConstraint::transform( double value ) const
{

  try
  {

    return std::log( _maximum - value );

  }
  GKG_CATCH( "double gkg::OpenLeftHalfLineOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenLeftHalfLineOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    return _maximum - std::exp( value );

  }
  GKG_CATCH( "double gkg::OpenLeftHalfLineOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenLeftHalfLineOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    return -std::exp( value );

  }
  GKG_CATCH( "double "
             "gkg::OpenLeftHalfLineOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string gkg::OpenLeftHalfLineOptimizerConstraint::displayType() const
{

  try
  {

    return "(-inf," + gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) +
           ")";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenLeftHalfLineOptimizerConstraint::displayType() const" );

}


//
// class OpenLeftHalfLineAlternativeOptimizerConstraint
//

gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::
                                 OpenLeftHalfLineAlternativeOptimizerConstraint(
                                                                double maximum )
                                                : gkg::OptimizerConstraint(),
                                                  _maximum( maximum )
{
}


gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::
                               ~OpenLeftHalfLineAlternativeOptimizerConstraint()
{
}


double gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    double tmp = _maximum - value;
    return 1.0 / ( 4.0 * tmp ) - tmp;

  }
  GKG_CATCH( "double gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    double tmp = std::sqrt( 1.0 + value * value );
    return _maximum + 0.5 * ( value - tmp );

  }
  GKG_CATCH( "double gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    double tmp = std::sqrt( 1.0 + value * value );
    return -0.5 * ( tmp - value ) / tmp;

  }
  GKG_CATCH( "double "
             "gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string 
gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::displayType() const
{

  try
  {

    return "(-inf," + gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) +
           ")";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenLeftHalfLineAlternativeOptimizerConstraint::"
             "displayType() const" );

}


//
// class ClosedIntervalOptimizerConstraint
//

gkg::ClosedIntervalOptimizerConstraint::ClosedIntervalOptimizerConstraint(
                                                                double minimum,
                                                                double maximum )
                                                : gkg::OptimizerConstraint(),
                                                  _minimum( minimum ),
                                                  _maximum( maximum )
{

  try
  {

    if ( maximum < minimum )
    {

      throw std::runtime_error( "maximum must be greater or equal to minimum" );

    }

  }
  GKG_CATCH( "gkg::ClosedIntervalOptimizerConstraint::"
             "ClosedIntervalOptimizerConstraint( "
             "double minimum, "
             "double maximum )" );
}


gkg::ClosedIntervalOptimizerConstraint::~ClosedIntervalOptimizerConstraint()
{
}


double gkg::ClosedIntervalOptimizerConstraint::transform( double value ) const
{

  try
  {

    if ( _minimum == _maximum )
    {

      return M_PI / 2.0;

    }
    return gkg::safeAsin( ( 2.0 * value - _maximum - _minimum ) /
                          ( _maximum - _minimum ) );

  }
  GKG_CATCH( "double gkg::OpenLeftHalfLineOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::ClosedIntervalOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    double tmp = std::sin( value );
    return 0.5 * ( _minimum * ( 1.0 - tmp ) + _maximum * ( 1.0 + tmp ) );

  }
  GKG_CATCH( "double gkg::ClosedIntervalOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::ClosedIntervalOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    return 0.5 * ( _maximum - _minimum ) * std::cos( value );

  }
  GKG_CATCH( "double "
             "gkg::ClosedIntervalOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


double gkg::ClosedIntervalOptimizerConstraint::constrainedDeltaParameter(
                                                   double deltaParameter ) const
{

  try
  {

    return deltaParameter * M_PI / ( _maximum - _minimum );

  }
  GKG_CATCH( "double gkg::OptimizerConstraint::constrainedDeltaParameter( "
             "double deltaParameter ) const" );


}




std::string gkg::ClosedIntervalOptimizerConstraint::displayType() const
{

  try
  {

    return "[" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) + "," +
           gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) + "]";

  }
  GKG_CATCH( "std::string "
             "gkg::ClosedIntervalOptimizerConstraint::displayType() const" );

}


//
// class OpenIntervalOptimizerConstraint
//

gkg::OpenIntervalOptimizerConstraint::OpenIntervalOptimizerConstraint(
                                                                double minimum,
                                                                double maximum )
                                     : gkg::OptimizerConstraint(),
                                       _minimum( minimum ),
                                       _maximum( maximum )
{

  try
  {

    if ( maximum < minimum )
    {

      throw std::runtime_error( "maximum must be greater or equal to minimum" );

    }

  }
  GKG_CATCH( "gkg::OpenIntervalOptimizerConstraint::"
             "OpenIntervalOptimizerConstraint( "
             "double minimum, "
             "double maximum )" );
}


gkg::OpenIntervalOptimizerConstraint::~OpenIntervalOptimizerConstraint()
{
}


double gkg::OpenIntervalOptimizerConstraint::transform( double value ) const
{

  try
  {

    return atanh( ( 2.0 * value - _maximum - _minimum ) /
                  ( _maximum - _minimum ) );

  }
  GKG_CATCH( "double gkg::OpenIntervalOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenIntervalOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    double tmp = std::tanh( value );
    return 0.5 * ( _minimum * ( 1.0 - tmp ) + _maximum * ( 1.0 + tmp ) );

  }
  GKG_CATCH( "double gkg::OpenIntervalOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenIntervalOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    double tmp = std::cosh( value );
    return 0.5 * ( _maximum - _minimum ) / ( tmp * tmp );

  }
  GKG_CATCH( "double "
             "gkg::OpenIntervalOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string gkg::OpenIntervalOptimizerConstraint::displayType() const
{

  try
  {

    return "(" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) + "," +
           gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) + ")";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenIntervalOptimizerConstraint::displayType() const" );

}


//
// class OpenIntervalAlternativeOptimizerConstraint
//

gkg::OpenIntervalAlternativeOptimizerConstraint::
                                     OpenIntervalAlternativeOptimizerConstraint(
                                                                double minimum,
                                                                double maximum )
                                                : gkg::OptimizerConstraint(),
                                                  _minimum( minimum ),
                                                  _maximum( maximum )
{

  try
  {

    if ( maximum < minimum )
    {

      throw std::runtime_error( "maximum must be greater or equal to minimum" );

    }

  }
  GKG_CATCH( "gkg::OpenIntervalAlternativeOptimizerConstraint::"
             "OpenIntervalAlternativeOptimizerConstraint( "
             "double minimum, "
             "double maximum )" );
}


gkg::OpenIntervalAlternativeOptimizerConstraint::
                                   ~OpenIntervalAlternativeOptimizerConstraint()
{
}


double gkg::OpenIntervalAlternativeOptimizerConstraint::transform(
                                                            double value ) const
{

  try
  {

    double tmp = ( 2.0 * value - _maximum - _minimum ) /
                 ( _maximum - _minimum );
    return tmp / std::sqrt( 1 - tmp * tmp );

  }
  GKG_CATCH( "double gkg::OpenIntervalAlternativeOptimizerConstraint::"
             "transform( "
             "double value ) const" );

}


double gkg::OpenIntervalAlternativeOptimizerConstraint::inverseTransform(
                                                            double value ) const
{

  try
  {

    double tmp = value / std::sqrt( 1.0 + value * value );
    return 0.5 * ( _minimum * ( 1.0 - tmp ) + _maximum * ( 1.0 + tmp ) );

  }
  GKG_CATCH( "double gkg::OpenIntervalAlternativeOptimizerConstraint::"
             "inverseTransform( "
             "double value ) const" );

}


double gkg::OpenIntervalAlternativeOptimizerConstraint::inverseDerivative(
                                                            double value ) const
{

  try
  {

    double tmp = ( 1.0 + value * value ) * std::sqrt( 1.0 + value * value );
    return 0.5 * ( _maximum - _minimum ) / tmp;

  }
  GKG_CATCH( "double "
             "gkg::OpenIntervalAlternativeOptimizerConstraint::"
             "inverseDerivative( "
             "double value ) const" );

}


std::string gkg::OpenIntervalAlternativeOptimizerConstraint::displayType() const
{

  try
  {

    return "(" + gkg::StringConverter::toString( _minimum, 'g', 10, 2 ) + "," +
           gkg::StringConverter::toString( _maximum, 'g', 10, 2 ) + ")";

  }
  GKG_CATCH( "std::string "
             "gkg::OpenIntervalAlternativeOptimizerConstraint::displayType() "
             "const" );

}



