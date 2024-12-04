#include <gkg-deep-nuclei-deepnuclei/CsfNormalizedSquareCenteredPhiFunction.h>


gkg::CsfNormalizedSquareCenteredPhiFunction
   ::CsfNormalizedSquareCenteredPhiFunction( double mean,
                                             double stdDev,
                                             double factor )
    : gkg::PhiFunction(),
      _mean( mean ),
      _stdDev( stdDev ),
      _factorOverTwo( factor / 2.0 )
{
}


gkg::CsfNormalizedSquareCenteredPhiFunction
   ::~CsfNormalizedSquareCenteredPhiFunction()
{
}


double gkg::CsfNormalizedSquareCenteredPhiFunction::getValue( double x ) const
{

  if ( x < _mean )
  {

    return 0.0;

  }
  else
  {

    double value = ( x - _mean ) / ( _stdDev );
    return _factorOverTwo * value * value;

  }

}
