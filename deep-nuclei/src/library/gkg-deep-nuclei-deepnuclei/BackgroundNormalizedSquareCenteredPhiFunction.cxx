#include <gkg-deep-nuclei-deepnuclei/BackgroundNormalizedSquareCenteredPhiFunction.h>
#include <algorithm>


gkg::BackgroundNormalizedSquareCenteredPhiFunction
   ::BackgroundNormalizedSquareCenteredPhiFunction( double grayMatterMean,
                                                   double grayMatterStdDev,
                                                   double whiteMatterMean,
                                                   double whiteMatterStdDev )
    : gkg::PhiFunction(),
      _grayMatterMean( grayMatterMean ),
      _grayMatterStdDev( grayMatterStdDev ),
      _whiteMatterMean( whiteMatterMean ),
      _whiteMatterStdDev( whiteMatterStdDev )
{
}


gkg::BackgroundNormalizedSquareCenteredPhiFunction
   ::~BackgroundNormalizedSquareCenteredPhiFunction()
{
}


double 
gkg::BackgroundNormalizedSquareCenteredPhiFunction::getValue( double x ) const
{

  double value1;
  double value2;

  if ( x < _whiteMatterMean )
  {

    double value = ( x - _whiteMatterMean ) / ( 2.0 * _whiteMatterStdDev );
    value1 = value * value;

  }
  else
  {

    value1 = -( x - _whiteMatterMean ) / ( 2.0 * _whiteMatterStdDev );

  }

  if ( x < _grayMatterMean )
  {

    double value = ( x - _grayMatterMean ) / ( 3.0 * _grayMatterStdDev );
    value2 = value * value;

  }
  else
  {

    double value = ( x - _grayMatterMean ) / _grayMatterStdDev;
    value2 = value * value;  

  }

  return std::min( std::min( value1, value2 ), 2.0 );


}
