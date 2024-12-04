#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelNelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::TwoPoolModelNelderMeadSimplexFunction::
TwoPoolModelNelderMeadSimplexFunction( const gkg::Matrix& qSpaceSamples, 
                                       const gkg::Vector& measuredAttenuations )
                                 : gkg::NelderMeadSimplexFunction( 13 ),
                                   _qSpaceSamples( qSpaceSamples ),
                                   _measuredAttenuations( measuredAttenuations )
{
}



gkg::TwoPoolModelNelderMeadSimplexFunction::
                                        ~TwoPoolModelNelderMeadSimplexFunction()
{
}


double gkg::TwoPoolModelNelderMeadSimplexFunction::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {

    double fDxx = parameters( 0 );
    double fDxy = parameters( 1 );
    double fDxz = parameters( 2 );
    double fDyy = parameters( 3 );
    double fDyz = parameters( 4 );
    double fDzz = parameters( 5 );

    double sDxx = parameters( 6 ) / 5.0;
    double sDxy = parameters( 7 ) / 5.0;
    double sDxz = parameters( 8 ) / 5.0;
    double sDyy = parameters( 9 ) / 5.0;
    double sDyz = parameters( 10 ) / 5.0;
    double sDzz = parameters( 11 ) / 5.0;

    double f = parameters( 12 ) * 670;

    int32_t trialCount = _measuredAttenuations.getSize();
    double discrepancy = 0.0;
    double error = 0.0;
    double ox = 0.0;
    double oy = 0.0;
    double oz = 0.0;
    double b = 0.0;

    int32_t trial = 0;
    for ( trial = 0; trial < trialCount; trial++ )
    {

      ox = _qSpaceSamples( trial, 0 );
      oy = _qSpaceSamples( trial, 1 );
      oz = _qSpaceSamples( trial, 2 );
      b = _qSpaceSamples( trial, 3 );

      error = ( _measuredAttenuations( trial ) -
                ( f * 
                  std::exp( -b * ( ox * ox * fDxx +
                                   2.0 * ox * oy * fDxy +
                                   2.0 * ox * oz * fDxz +
                                   oy * oy * fDyy +
                                   2.0 * oy * oz * fDyz +
                                   oz * oz * fDzz ) ) +
                  ( 1 - f ) *
                  std::exp( -b * ( ox * ox * sDxx +
                                   2.0 * ox * oy * sDxy +
                                   2.0 * ox * oz * sDxz +
                                   oy * oy * sDyy +
                                   2.0 * oy * oz * sDyz +
                                   oz * oz * sDzz ) ) ) );

      discrepancy += error * error;

    }

    return discrepancy;

  }
  GKG_CATCH( "double gkg::TwoPoolModelNelderMeadSimplexFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}
