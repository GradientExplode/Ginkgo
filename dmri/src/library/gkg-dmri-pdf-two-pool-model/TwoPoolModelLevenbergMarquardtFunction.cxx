#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelLevenbergMarquardtFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::TwoPoolModelLevenbergMarquardtFunction::
                                         TwoPoolModelLevenbergMarquardtFunction(
                                                 const gkg::Vector& parameters )
                                            : gkg::LevenbergMarquardtFunction(
                                                                    parameters )
{
}


gkg::TwoPoolModelLevenbergMarquardtFunction::
                                       ~TwoPoolModelLevenbergMarquardtFunction()
{
}


double gkg::TwoPoolModelLevenbergMarquardtFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    double fDxx = p( 0 );
    double fDxy = p( 1 );
    double fDxz = p( 2 );
    double fDyy = p( 3 );
    double fDyz = p( 4 );
    double fDzz = p( 5 );

    double sDxx = p( 6 ) / 10.0;
    double sDxy = p( 7 ) / 10.0;
    double sDxz = p( 8 ) / 10.0;
    double sDyy = p( 9 ) / 10.0;
    double sDyz = p( 10 ) / 10.0;
    double sDzz = p( 11 ) / 10.0;

    double f = p( 12 ) * 1000.0;

    double ox = x( 0 );
    double oy = x( 1 );
    double oz = x( 2 );
    double b = x( 3 );


    return ( f * 
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
                              oz * oz * sDzz ) ) );

  }
  GKG_CATCH( "double gkg::TwoPoolModelLevenbergMarquardtFunction::"
             "getValueWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


gkg::Vector 
gkg::TwoPoolModelLevenbergMarquardtFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    gkg::Vector derivative( 13 );

    double fDxx = p( 0 );
    double fDxy = p( 1 );
    double fDxz = p( 2 );
    double fDyy = p( 3 );
    double fDyz = p( 4 );
    double fDzz = p( 5 );

    double sDxx = p( 6 ) / 10.0;
    double sDxy = p( 7 ) / 10.0;
    double sDxz = p( 8 ) / 10.0;
    double sDyy = p( 9 ) / 10.0;
    double sDyz = p( 10 ) / 10.0;
    double sDzz = p( 11 ) / 10.0;

    double f = p( 12 ) * 1000.0;

    double ox = x( 0 );
    double oy = x( 1 );
    double oz = x( 2 );
    double b = x( 3 );



    double fastFractionExponential = std::exp( -b * ( ox * ox * fDxx +
                                                      2.0 * ox * oy * fDxy +
                                                      2.0 * ox * oz * fDxz +
                                                      oy * oy * fDyy +
                                                      2.0 * oy * oz * fDyz +
                                                      oz * oz * fDzz ) );

    double slowFractionExponential = std::exp( -b * ( ox * ox * sDxx +
                                                      2.0 * ox * oy * sDxy +
                                                      2.0 * ox * oz * sDxz +
                                                      oy * oy * sDyy +
                                                      2.0 * oy * oz * sDyz +
                                                      oz * oz * sDzz ) );

    derivative( 0 )  = - f * b * ox * ox * fastFractionExponential;
    derivative( 1 )  = - f * b * 2 * ox * oy * fastFractionExponential;
    derivative( 2 )  = - f * b * 2 * ox * oz * fastFractionExponential;
    derivative( 3 )  = - f * b * oy * oy * fastFractionExponential;
    derivative( 4 )  = - f * b * 2 * oy * oz * fastFractionExponential;
    derivative( 5 )  = - f * b * oz * oz * fastFractionExponential;

    derivative( 6 )  = f * b* ox * ox * slowFractionExponential;
    derivative( 7 )  = f * b * 2 * ox * oy * slowFractionExponential;
    derivative( 8 )  = f * b * 2 * ox * oz * slowFractionExponential;
    derivative( 9 )  = f * b * oy * oy * slowFractionExponential;
    derivative( 10 ) = f * b * 2 * oy * oz * slowFractionExponential;
    derivative( 11 ) = f * b * oz * oz * slowFractionExponential;

    derivative( 12 ) = fastFractionExponential - slowFractionExponential;

    return derivative;

  }
  GKG_CATCH( "gkg::Vector "
             "gkg::TwoPoolModelLevenbergMarquardtFunction::"
             "getDerivativeWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}




