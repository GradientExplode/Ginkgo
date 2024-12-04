#include <gkg-dmri-odf/SHFiberOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


#define DEFAULT_REGULARIZATION_ORIENTATION_COUNT 300


gkg::SHFiberOdf::SHFiberOdf( const gkg::OrientationSet* orientationSet )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet )
{
}


gkg::SHFiberOdf::~SHFiberOdf()
{
}

// direct implementation from Tournier et al NeuroImage 2007
// lambda, tau, convergence are parameters... should be able to modify them
void gkg::SHFiberOdf::applyConstrainedSuperResolutionRegularization(
                                       const gkg::Vector& normalizedSignal,
                                       const gkg::Matrix& B,
                                       const gkg::Matrix& regularizationMatrix,
                                       const gkg::Matrix& convolutionMatrix,
                                       bool superResolution )
{

  try
  {

    bool debug = false; 

    int32_t R = B.getSize1();  // # input/output directions 
    int32_t N = B.getSize2();  // # SH coefficients
    int32_t M = regularizationMatrix.getSize1();  // # regularization directions
    int32_t orientationCount = normalizedSignal.getSize();
    
    gkg::Vector fOdf( N ), fOdf_n( N );

    //
    // INITIALIZATION of fiber ODF
    // initialize to order 4 estimation of the fiber ODF
    //
    int32_t i,j;
    for ( i = 0; i < N; i++ )
    {

      if ( i < 15 )
      {

	fOdf( i ) =  _values[ i ];

      }
      else
      {

        fOdf( i ) = 0.0;

      }      

    }
    
    if ( debug )
    {

      std::cout << "Initial fOdf : " << fOdf << std::endl;

    }

    //
    // project fOdf to the sphere, sphericalFunction is of size M
    //
    gkg::Vector
      sphericalFunction = regularizationMatrix.getComposition( fOdf );

    //
    // find mean value of the spherical function and
    // find if there are negative values on the sphere
    //
    // New regularization matrix L is defined such that
    // L(i,j) = B(i,j) if spherical function is negative along orientation j
    //        = otherwise
    //
    // In practice, we find orientations along where sphericalFunction is < tau
    // where tau = x * mean( sphericalFunction )
    // by default, x = 0.1, i.e. 10%
    //
    double mean = 0.0;

    // j goes from 0 to M-1, i goes from 0 to N-1
    for ( j = 0; j < M; j++ ) 
    {

      mean += sphericalFunction( j );

    }
    mean = mean / M;

    if ( debug )
    {

      std::cout << "Mean value on the sphere is : " << mean << std::endl;

    }

    double tau = mean * 0.1;
    int32_t count = 0;
    bool neg = false; //assume no negative values on the sphere
    gkg::Matrix L = regularizationMatrix;
    for ( j = 0; j < M; j++ ) 
    {

      if ( sphericalFunction( j ) >= tau )
	{

	for ( i = 0; i < N; i++ ) // loop over number of SH coefficients
        {

          L( j, i ) = 0.0;

        }

      }
      else
      {

        ++ count;
        neg = true;  //there are neg values on the sphere

      }

    }

    if ( debug )
    {

      std::cout << "Initially, there are " 
                << count << " negative values on the sphere..." << std::endl;
    }

    //
    //  SET REGULARIZATION LAMBDA
    //
    // scale lambda to account for differences in the number of
    // SH coefficients and number of mapped directions;
    //
    double lambda = 0.1;   // default parameter in Tournier et al 2007 is one
                           // 0.1 is better in my experiments.
                           // could be an input
    double lamb = lambda * N * convolutionMatrix( 0, 0 ) / M;

    if ( debug )
    {

      std::cout << " lambda, lamb " << lambda << "," << lamb << std::endl;
      
    }

    //
    //  iterate to remove negative values & sharp
    //
    gkg::Matrix A = B * convolutionMatrix;  // R by N matrix
    int32_t t = 0;
    int32_t convergence = 50; // could be given by user

    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    gkg::Matrix Aplus( N, R );
    // L2     is of size M x N
    // M      is of size (R + M) x N 
    // Mplus  is of size N x (R + M) 
    // bprime is of size (R + M) x 1 ;
    gkg::Matrix L2 = L*lamb;
    gkg::Matrix Mmatrix( A.getSize1() + L.getSize1(), A.getSize2() );
    gkg::Matrix Mplus( A.getSize2(), A.getSize1() + L.getSize1() );
    gkg::Vector bprime( A.getSize1() + L.getSize1() );

    while ( ( t < convergence ) && neg )
    {

      neg = false;

      //
      //   Sanity Check for super resolution, number of negative values on
      //   the sphere plus the number input directions must exceed the 
      //   number of SH coefficients
      //
      if ( superResolution && 
           ( ( int32_t )_values.size() > ( count + orientationCount ) ) )
      {

        break;

      }

      if ( superResolution )
      {

        L2 = L*lamb;
        getSuperResolutionMatrices( A, L2, normalizedSignal, Mmatrix, bprime );
        factory->getMoorePenrosePseudoInverse( Mmatrix, Mplus );
        fOdf = Mplus.getComposition( bprime );

      }
      else
      {
	
        factory->getThikonovPseudoInverse( 
                                      A,
                                      L.getTransposition().getComposition( L ),
                                      lamb * lamb,
                                      Aplus);
        fOdf = Aplus.getComposition( normalizedSignal );

      }

      if ( debug )
      {

        std::cout << "fOdf : " << fOdf << std::endl;
	
      }
      sphericalFunction = regularizationMatrix.getComposition( fOdf );
      gkg::Matrix newL = regularizationMatrix;
      count = 0;
      for( j = 0; j < M; j++ )
      {

        if ( sphericalFunction( j ) >= tau )
        {

          for ( i = 0; i < N; i++ )
          {

            newL( j, i ) = 0.0;

          }

        }
        else
        {

          neg = true;  //there are neg values on the sphere
          ++ count;

        }

      }

      if ( std::operator==( L, newL ) )
      {

        if ( debug )
        {

          std::cout << "L and newL are the same... stop iterations"
                    << std::endl;

        }
        neg = false;

      }
      else
      {
      
	L = newL;
	fOdf_n = fOdf;

      }
      ++ t;

    }

    if ( debug ) 
    {

      std::cout << "Convergence attained after "
                << t << " iterations..."
                << std::endl;
      count = 0;
      for ( j = 0; j < M; j++ )
      {

        if ( sphericalFunction( j ) < 0 )
        {

          ++ count;

        }

      }

      std::cout << "At the end, there are "
                << count
                << " negative values on the sphere..."
                << std::endl;

    }

    // copying final SH coefficients
    for ( i = 0; i < N; i++ )
    {

      _values[ i ] = ( float )fOdf( i );//fOdf_n( i );

    }

   }
  GKG_CATCH( "void gkg::SHFiberOdf::"
             "applyConstrainedSuperResolutionRegularization()" );

}


void gkg::SHFiberOdf::getDeconvolutionMatrices(
                                   const gkg::DiffusionKernel& diffusionKernel,
                                   int32_t maximumSphericalHarmonicOrder,
                                   double bValue,
                                   gkg::Matrix& regularizationMatrix,
                                   gkg::Matrix& convolutionMatrix )
{

  try
  {

    //
    // get orientation set for 300 directions on the sphere
    // and build basis matrix Bregularization
    //
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // number of SH coefficients
    int32_t N = (int32_t) ( ( maximumSphericalHarmonicOrder + 1 )*
                            ( maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // number of regularization orientations
    int32_t regularizationOrientationCount =
      DEFAULT_REGULARIZATION_ORIENTATION_COUNT;
    
    gkg::OrientationSet regularizationOrientationSet(
      gkg::ElectrostaticOrientationSet( regularizationOrientationCount / 2 ).
                                              getSymmetricalOrientations() );

    int32_t n, o;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;

    regularizationMatrix.reallocate( regularizationOrientationCount, N );
    for ( o = 0; o < regularizationOrientationCount; o++ )
    {

      orientation = regularizationOrientationSet.getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        regularizationMatrix( o, n ) =
          factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    gkg::Matrix regularizationMatrixPlus( N, regularizationOrientationCount );
    factory->getMoorePenrosePseudoInverse( regularizationMatrix,
                                           regularizationMatrixPlus );

    //
    //    Build Convolution Matrix 
    //
    //    If the fiber ODF was a unit delta function aligned with the
    //    z-axis, then the DW signal measured should be identical to
    //    the response function R. In other words, if f_l corresponds to a
    //    delta function, then s_l must correspond to the response function.
    //    In this case, as both functions are axially symmetric, all m != 0
    //    coefficients are zero. Therefore, r_l can be computed by simple
    //    scalar division of the lth order, m = 0 SH coefficients of R by
    //    the corresponding coefficient for a delta function.
    //    The Rot matrix R is simply R_jj = r_l
    //
    //

    //
    //  1 Fiber Synthetic Signal Generation
    //
    gkg::Matrix rotationMatrix( 3, 3 );
    rotationMatrix.setIdentity();

    gkg::Vector coefficients( 6 );
    gkg::Vector eigenValues = diffusionKernel.getTensor().getEigenValues();
    coefficients( 0 ) = eigenValues( 2 ) * pow( 10.0, 6.0 );
    coefficients( 1 ) = 0.0;
    coefficients( 2 ) = 0.0;
    coefficients( 3 ) = eigenValues( 1 ) * pow( 10.0, 6.0 );
    coefficients( 4 ) = 0.0;
    coefficients( 5 ) = eigenValues( 0 ) * pow( 10.0, 6.0 );  //along z

    gkg::Tensor generatingTensor( 0, coefficients );

    // need b-value in 10^-6 s/m^2
    gkg::Vector signal = generatingTensor.getRotatedDiffusionWeightedSignal(
                                               regularizationOrientationSet,
                                               rotationMatrix,
                                               bValue,
                                               1.0 );

    gkg::Vector sphericalHarmonicSynthetic1FiberSignal = 
                             regularizationMatrixPlus.getComposition( signal );

    gkg::Vector diracFunction( N );
    gkg::Vector nonZeroSphericalHarmonicSynthetic1FiberSignal( 
                                      maximumSphericalHarmonicOrder / 2 + 1 );
    gkg::Vector nonZeroDiracFunction( maximumSphericalHarmonicOrder / 2 + 1 );

    int32_t i = 0;
    int32_t l, m;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      diracFunction( n ) = factory->getDiracCoefficient( n, 0, 0 );

      if ( m == 0 )
      {

        nonZeroSphericalHarmonicSynthetic1FiberSignal( i ) = 
                                   sphericalHarmonicSynthetic1FiberSignal( n );
        nonZeroDiracFunction( i ) = diracFunction( n );
        i++;

      }

    }

    convolutionMatrix.reallocate( N, N );
    convolutionMatrix.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      // R(n n) = syntheticSignal(l, m=0) / Dirac(l,m)
      convolutionMatrix( n, n ) =
        nonZeroSphericalHarmonicSynthetic1FiberSignal( l / 2 ) /
        nonZeroDiracFunction( l / 2 );

    }

  }
  GKG_CATCH( "void gkg::SHFiberOdf::getDeconvolutionMatrices( "
             "const gkg::DiffusionKernel& diffusionKernel, "
             "int32_t maximumSphericalHarmonicOrder, "
             "double bValue, "
             "gkg::Matrix& regularizationMatrix, "
             "gkg::Matrix& convolutionMatrix )" );

}



//See Tournier 2007 Super-resolution
void gkg::SHFiberOdf::getSuperResolutionMatrices( const gkg::Matrix& A,
                                                  const gkg::Matrix& L2,
                                                  const gkg::Vector& signal,
                                                  gkg::Matrix& M,
                                                  gkg::Vector& bPrime )
{

  try
  {

    int32_t i, j;
    M.reallocate( A.getSize1() + L2.getSize1(), A.getSize2() );
    bPrime.reallocate( A.getSize1() + L2.getSize1() );

    int32_t index = 0;
    for ( i = 0; i < M.getSize1(); i++ )
    {

      for ( j = 0; j < M.getSize2(); j++ )
      {

        if ( i < A.getSize1() )
        {

          M( i, j ) = A( i, j );
          bPrime( i ) = signal( i );

        }
        else
        {

          M( i, j ) = L2( index, j );
          bPrime( i ) = 0.0;

        }

      }

      // increment only in the second part of matrix M
      if ( i >= A.getSize1() )
      {

        ++ index;

      }

    }

  }
  GKG_CATCH( "void gkg::SHFiberOdf::getSuperResolutionMatrices( "
             "const gkg::Matrix& A, "
             "const gkg::Matrix& L2, "
             "const gkg::Vector& signal, "
             "gkg::Matrix& M, "
             "gkg::Vector& bPrime )" );

}


#undef DEFAULT_REGULARIZATION_ORIENTATION_COUNT
