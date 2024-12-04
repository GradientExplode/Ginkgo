#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <cmath>


//
// class PhiFunction
//

gkg::PhiFunction::PhiFunction()
{
}


gkg::PhiFunction::~PhiFunction()
{
}


//
// class IdentityPhiFunction
//

gkg::IdentityPhiFunction::IdentityPhiFunction()
                         : gkg::PhiFunction()
{
}


gkg::IdentityPhiFunction::~IdentityPhiFunction()
{
}


double gkg::IdentityPhiFunction::getValue( double x ) const
{

  return x;

}


//
// class NormalizedSquaredCenterPhiFunction
//

gkg::NormalizedSquaredCenterPhiFunction::NormalizedSquaredCenterPhiFunction(
                                                   double mean,
                                                   double standardDeviation,
                                                   double factor )
                                        : gkg::PhiFunction(),
                                          _mean( mean ),
                                          _standardDeviation(
                                                         standardDeviation ),
                                          _factorOverTwo( factor / 2.0 )
{
}


gkg::NormalizedSquaredCenterPhiFunction::~NormalizedSquaredCenterPhiFunction()
{
}


double gkg::NormalizedSquaredCenterPhiFunction::getValue( double x ) const
{

  double value = ( x - _mean ) / _standardDeviation;
  return _factorOverTwo * value * value;

}


//
// class GaussianPhiFunction
//

gkg::GaussianPhiFunction::GaussianPhiFunction( double sigma, double maximumX )
                         : gkg::PhiFunction(),
                           _twoTimesSigma2( 2.0 * sigma * sigma ),
                           _maximumX( maximumX )
{
}


gkg::GaussianPhiFunction::~GaussianPhiFunction()
{
}


double gkg::GaussianPhiFunction::getValue( double x ) const
{

  if ( std::abs( x ) > _maximumX )
  {

    return 0.0;

  }
  return std::exp( - x * x / _twoTimesSigma2 );

}


//
// class MultiquadricPhiFunction
//

gkg::MultiquadricPhiFunction::MultiquadricPhiFunction( double sigma,
                                                       double maximumX )
                             : gkg::PhiFunction(),
                               _sigma2( sigma * sigma ),
                               _maximumX( maximumX )
{
}


gkg::MultiquadricPhiFunction::~MultiquadricPhiFunction()
{
}


double gkg::MultiquadricPhiFunction::getValue( double x ) const
{

  if ( std::abs( x ) > _maximumX )
  {

    return 0.0;

  }
  return 1.0 / std::sqrt( x * x + _sigma2 );

}


//
// class ThinPlateSplinePhiFunction
//

gkg::ThinPlateSplinePhiFunction::ThinPlateSplinePhiFunction( double sigma,
                                                             double maximumX )
                             : gkg::PhiFunction(),
                               _sigmaTimesTwo( 2.0 * sigma ),
                               _maximumX( maximumX )
{
}


gkg::ThinPlateSplinePhiFunction::~ThinPlateSplinePhiFunction()
{
}


double gkg::ThinPlateSplinePhiFunction::getValue( double x ) const
{

  double absoluteValue = std::abs( x );
  if ( absoluteValue > _maximumX )
  {

    return 0.0;

  }
  return std::pow( absoluteValue, _sigmaTimesTwo ) * std::log( absoluteValue );

}
