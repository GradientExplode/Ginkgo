#include <gkg-dmri-noise-correction/SingleShellSHRicianNoiseCorrection.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>



#define EPSILON 1e-6


//
// class SingleShellSHRician
//

gkg::SingleShellSHRicianNoiseFunction::SingleShellSHRicianNoiseFunction(
                                                   const gkg::Matrix& B,
                                                   const gkg::Vector& signal,
                                                   float noiseStdDev )
                           : gkg::FletcherReevesConjugateGradientFunction(
                                                                B.getSize2() ),
                             gkg::NelderMeadSimplexFunction( B.getSize2() ),
                             _B( B ),
                             _signal( signal ),
                             _noiseStdDev( noiseStdDev )
{
}


gkg::SingleShellSHRicianNoiseFunction::~SingleShellSHRicianNoiseFunction()
{
}


// Our correction is based on the Maximum Likelihood estimation.
// To obtain this maximum, we need to minimize a so-called energy.
// Here are the main equations of our process:
// d/dCj ( logL ) = 1/sigma^2* sum (i=1 to M ) {
//                  B(i,j) ( bessi1( z )/bessi0( z ) * signal( i ) - A )  }
//
// with:
//
// A = sum (j=1 to N ) { B(i,j) * Cj }
// z = signal( i )/( sigma^2 ) * A    for each orientation i
//
// Cj = parameters(j)
// M = orientationCount
// N = parameterCount
//
// As B(i,j) is a strickly positive matrix, we need to have:
//
// energy = sum (i=1 to M ) {
//                   ( bessi1( z )/bessi0( z ) * signal( i ) - A )^2 } = 0
//
// We also define derivative as d/dCj ( energy ).
// The vector derivative indicates us where we are on the curve of energy.
//
// derivative( j ) = sum (i=1 to M ) {
//                    2 * ( bessi1( z )/bessi0( z ) * signal( i ) - A )*
//                    B(i,j) * (  signal( i ) * signal( i ) / ( sigma^2 ) *
//                         ( 1 - bessi1( z ) /  z /bessi0( z ) -
//                           ( bessi1( z ) / bessi0( z ) )^2 )
//                    - 1  )
//                                   }
// 


double gkg::SingleShellSHRicianNoiseFunction::getValueAt(
                                          const gkg::Vector& parameters ) const
{

  try
  {

    double z = 0.0;       // Bessel function argument
    double A = 0.0;       // signal
    double energy = 0.0;
    double epsilon = 0.0;

    int32_t o, p;
    for ( o = 0; o < _B.getSize1(); o++ )
    {

      if ( _signal( o ) > 0.0 )
      {

        A = 0.0;
        for ( p = 0; p < _B.getSize2(); p++ )
        {

          // get the diffusion signal for one orientation o
          A +=  _B( o, p ) * parameters( p );

        }
        // get z for one orientation i
        z = ( _signal( o ) / ( _noiseStdDev * _noiseStdDev ) ) * A;


        epsilon = ( alpha( z ) * _signal( o ) - A );
        energy += epsilon * epsilon;

      }

    }
    return energy;

  }
  GKG_CATCH( "double gkg::SingleShellSHRicianNoiseFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


gkg::Vector gkg::SingleShellSHRicianNoiseFunction::getDerivativeAt(
                                          const gkg::Vector& parameters ) const
{

  try
  {

    double z = 0.0;
    double A=0.0;
    double alphaz = 0.0;
    double squareNoiseStdDev = _noiseStdDev * _noiseStdDev;

    gkg::Vector derivative( _B.getSize2() );
    derivative.setZero();

    int32_t o, p;
    for ( o = 0; o < _B.getSize1(); o++ )
    {

      if ( _signal( o ) > 0.0 )
      {

        A = 0.0;
        for ( p = 0; p < _B.getSize2(); p++ )
        {

          // get the diffusion signal for one orientation o
          A +=  _B( o, p ) * parameters( p );

        }

        // get z for one orientation i
        z = _signal( o ) * A / squareNoiseStdDev;
        alphaz = alpha( z );

        for ( p = 0; p < _B.getSize2(); p++ )
        {

          derivative( p ) += 2 * ( alphaz * _signal( o ) - A ) *
                             _B( o, p ) *
                             ( ( ( _signal( o ) * _signal( o ) ) /
                                 squareNoiseStdDev ) *
                               ( 1 - alphaz / z - alphaz * alphaz )
                               - 1 ) ;

        }

      }

    }
    return derivative;

  }
  GKG_CATCH( "gkg::Vector gkg::SingleShellSHRicianNoiseFunction::getDerivativeAt( "
             "const gkg::Vector& parameters ) const" );

}


double gkg::SingleShellSHRicianNoiseFunction::getValueAndDerivativeAt( 
                                                const gkg::Vector& parameters,
                                                gkg::Vector& derivative ) const
{

  try
  {

    derivative = getDerivativeAt( parameters );
    return getValueAt( parameters );

  }
  GKG_CATCH( "double gkg::SingleShellSHRicianNoiseFunction::"
             "getValueAndDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


double gkg::SingleShellSHRicianNoiseFunction::alpha( double z )
{

  try
  {

    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    double value = 0.0;
    if ( z <= 10.0 )
    {

      value = factory->getRegularBesselFunctionI1( z ) /
              factory->getRegularBesselFunctionI0( z );

    }
    else
    {

      double z2 = z * z;
      double z3 = z2 * z;
      double z4 = z3 * z;
      double z5 = z4 * z;
      double z6 = z5 * z;
      double z7 = z6 * z;
      double z8 = z7 * z;
      double z9 = z8 * z;
      double z10 = z9 * z;
      double z11 = z10 * z;

      value = 1.0
              - 0.5 / z
              - 0.125 / z2
              - 0.125 / z3
              - 0.1953125 / z4
              - 0.40625 / z5
              - 1.0478515625 / z6
              - 3.21875 / z7
              - 11.46646118164 / z8
              - 46.478515625 / z9
              - 211.276149749755 / z10
              - 1064.67822265625 / z11;

    }
    return value;

  }
  GKG_CATCH( "double gkg::SingleShellSHRicianNoiseFunction::alpha( "
             "double z )" );

}


//
// class SingleShellSHRicianNoiseCorrection
//

gkg::SingleShellSHRicianNoiseCorrection::SingleShellSHRicianNoiseCorrection(
                                     const gkg::Volume< float >& t2,
                                     const gkg::Volume< float >& dw,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t maximumSphericalHarmonicOrder,
                                     float regularizationLCurveFactor,
                                     float noiseStdDev,
                                     int32_t maximumIterationCount,
                                     float stepSize,
                                     float maximumTestGradient,
                                     float maximumTolerance )
{

  try
  {

    // building SH basis direct and inverse matrix
    gkg::Matrix B;
    gkg::Matrix Bplus;
    getReconstructionMatrix( dw,
                             maximumSphericalHarmonicOrder,
                             regularizationLCurveFactor,
                             B,
                             Bplus );

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;
    if ( averageT2 <= 0 )
    {

      averageT2 = EPSILON;

    }

    // building normalized signal vector
    gkg::Vector normalizedSignal( orientationCount );
    for ( t = 0; t < orientationCount; t++ )
    {

      float dwValue = dw( site, t );
      normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

    }

    // processing DW spherical harmonics coefficient(s)
    // that is our initial set of parameters
    _dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                           normalizedSignal );

    // processing the Rician noise correction if enough signal
    if ( normalizedSignal.getMean() > std::sqrt( 2 ) * noiseStdDev )
    {

      gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      gkg::SingleShellSHRicianNoiseFunction shRicianNoiseFunction( B,
                                                      normalizedSignal,
                                                      noiseStdDev );
      factory->getFletcherReevesConjugateGradientOptimumParameters(
                                             _dwSphericalHarmonicsCoefficients,
                                             shRicianNoiseFunction,
                                             maximumIterationCount,
                                             stepSize,
                                             maximumTestGradient,
                                             maximumTolerance,
                                             false );
      _corrected = true;

    }
    else
    {

      _corrected = false;

    }

    // processing denoised DW signal values
    _denoisedDw.reallocate( orientationCount );
    _denoisedDw = B.getComposition( _dwSphericalHarmonicsCoefficients );
    _denoisedDw = _denoisedDw * ( double )averageT2;

  }
  GKG_CATCH( "gkg::SingleShellSHRicianNoiseCorrection::SingleShellSHRicianNoiseCorrection( "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "float noiseStdDev, "
             "int32_t maximumIterationCount, "
             "float stepSize, "
             "float maximumTestGradient, "
             "float maximumTolerance )" );

}


gkg::SingleShellSHRicianNoiseCorrection::SingleShellSHRicianNoiseCorrection(
                                     const gkg::Volume< float >& t2,
                                     const gkg::Volume< float >& dw,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t maximumSphericalHarmonicOrder,
                                     float regularizationLCurveFactor,
                                     float noiseStdDev,
                                     const std::vector< float >& deltaParameters,
                                     int32_t maximumIterationCount,
                                     float maximumTestSize )
{

  try
  {

    // building SH basis direct and inverse matrix
    gkg::Matrix B;
    gkg::Matrix Bplus;
    getReconstructionMatrix( dw,
                             maximumSphericalHarmonicOrder,
                             regularizationLCurveFactor,
                             B,
                             Bplus );

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;
    if ( averageT2 <= 0 )
    {

      averageT2 = EPSILON;

    }

    // building normalized signal vector
    gkg::Vector normalizedSignal( orientationCount );
    for ( t = 0; t < orientationCount; t++ )
    {

      float dwValue = dw( site, t );
      normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

    }

    // processing DW spherical harmonics coefficient(s)
    // that is our initial set of parameters
    _dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                           normalizedSignal );

    // processing the Rician noise correction if enough signal
    if ( normalizedSignal.getMean() > std::sqrt( 2 ) * noiseStdDev )
    {

      gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      gkg::SingleShellSHRicianNoiseFunction shRicianNoiseFunction( B,
                                                      normalizedSignal,
                                                      noiseStdDev );
      gkg::Vector vectorOfDeltaParameters( deltaParameters );
      double testSize = 0.0;
      factory->getNelderMeadSimplexOptimumParameters(
                                             _dwSphericalHarmonicsCoefficients,
                                             vectorOfDeltaParameters,
                                             testSize,
                                             shRicianNoiseFunction,
                                             maximumIterationCount,
                                             ( double )maximumTestSize,
                                             false );
      _corrected = true;

    }
    else
    {

      _corrected = false;

    }

    // processing denoised DW signal values
    _denoisedDw.reallocate( orientationCount );
    _denoisedDw = B.getComposition( _dwSphericalHarmonicsCoefficients );
    _denoisedDw = _denoisedDw * ( double )averageT2;

  }
  GKG_CATCH( "gkg::SingleShellSHRicianNoiseCorrection::SingleShellSHRicianNoiseCorrection( "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "float noiseStdDev, "
             "int32_t maximumIterationCount, "
             "float stepSize, "
             "float maximumTestGradient, "
             "float maximumTolerance )" );

}


gkg::SingleShellSHRicianNoiseCorrection::~SingleShellSHRicianNoiseCorrection()
{
}


const gkg::Vector& gkg::SingleShellSHRicianNoiseCorrection::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::SingleShellSHRicianNoiseCorrection::"
	     "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Vector& gkg::SingleShellSHRicianNoiseCorrection::getDenoisedDw() const
{

  try
  {

    return _denoisedDw;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::SingleShellSHRicianNoiseCorrection::"
	     "getDenoisedDw() const" );

}


bool gkg::SingleShellSHRicianNoiseCorrection::isCorrected() const
{

  try
  {

    return _corrected;

  }
  GKG_CATCH( "bool "
             "gkg::SingleShellSHRicianNoiseCorrection::"
	     "isCorrected() const" );

}


void gkg::SingleShellSHRicianNoiseCorrection::getReconstructionMatrix(
                                const gkg::HeaderedObject& headeredObject,
                                int32_t maximumSphericalHarmonicOrder,
                                float regularizationLCurveFactor,
                                gkg::Matrix& B,
                                gkg::Matrix& Bplus )
{

  try
  {

    // getting orientation count
    int32_t orientationCount = 0;

    if ( headeredObject.getHeader().hasAttribute( "sizeT" ) )
    {

      headeredObject.getHeader().getAttribute( "sizeT", orientationCount );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeT'" );

    }

    // building orientation matrix
    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    if ( headeredObject.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }

    std::vector< gkg::Vector3d< float > > orientations( orientationCount );
    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();

      orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

    }
    gkg::OrientationSet inputOrientationSet( orientations );

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
                  ( maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // measure count
    int32_t M = inputOrientationSet.getCount();

    // building matrix of spherical harmonics on input orientation(s)
    int32_t n, m;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;

    B.reallocate( M, N );

    for ( m = 0; m < M; m++ )
    {

      orientation = inputOrientationSet.getOrientation( m );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Laplace-Beltrami smoothness matrix for regularization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L )^(-1) B^T
    Bplus.reallocate( N, M );
    factory->getThikonovPseudoInverse( B, L,
                                       ( double )regularizationLCurveFactor,
                                       Bplus );

  }
  GKG_CATCH( "void gkg::SingleShellSHRicianNoiseCorrection::"
             "getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "int32_t maximumSphericalHarmonicOrder, "
             "gkg::Matrix& B, )"
             "gkg::Matrix& Bplus );")

}


#undef EPSILON
