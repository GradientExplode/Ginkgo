#include <gkg-dmri-odf-sd/SphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class S >
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf( 
                                  const gkg::OrientationSet* orientationSet,
                                  const gkg::Volume< float >& t2,
                                  const gkg::Volume< float >& dw,
                                  const gkg::Vector3d< S >& site,
                                  const gkg::DiffusionKernel& diffusionKernel,
                                  int32_t maximumSphericalHarmonicOrder,
                                  const gkg::Vector& filteringCoefficients,
                                  bool constrainedSphericalDeconvolution,
                                  bool superResolution )
                                    : gkg::SHFiberOdf( orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      // collecting b-value
      std::vector< double > bValues;
      if ( dw.getHeader().hasAttribute( "bvalues" ) )
      {

        dw.getHeader().getAttribute( "bvalues", bValues );

      }
      else
      {

        throw std::runtime_error( "missing 'bvalues' attribute" );

      }
      gkg::AverageFilter< std::vector< double >, double > averageFilter;
      double averageBValue = 0.0;
      averageFilter.filter( bValues, averageBValue );

      // building reconstruction matrices
      gkg::Matrix reconstructionMatrix;
      gkg::Matrix B;
      gkg::Matrix Bplus;

      getReconstructionMatrix( dw,
                               diffusionKernel,
                               maximumSphericalHarmonicOrder,
                               filteringCoefficients,
                               averageBValue,
                               reconstructionMatrix,
                               B,
                               Bplus );

      gkg::Vector normalizedSignal;
      this->reconstructUnregularized( orientationSet,
                                      t2,
                                      dw,
                                      site,
                                      reconstructionMatrix,
                                      Bplus,
                                      normalizedSignal );

      // applying regularization
      if ( constrainedSphericalDeconvolution )
      {

        gkg::Matrix regularizationMatrix;
        gkg::Matrix convolutionMatrix;

        getDeconvolutionMatrices( diffusionKernel,
                                  maximumSphericalHarmonicOrder,
                                  averageBValue,
                                  regularizationMatrix,
                                  convolutionMatrix );

        applyConstrainedSuperResolutionRegularization(
                                  normalizedSignal,
                                  B,
                                  regularizationMatrix,
                                  convolutionMatrix,
                                  superResolution );

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::SphericalDeconvolutionFiberOdf::"
             "SphericalDeconvolutionFiberOdf( "
             "const gkg::OrientationSet* orientationSet,"
             "const gkg::Volume< float >& t2,"
             "const gkg::Volume< float >& dw,"
             "const gkg::Vector3d< S >& site,"
             "const gkg::DiffusionKernel& kernel,"
             "int32_t maximumSphericalHarmonicOrder,"
             "const Vector& filteringCoefficients,"
             "bool constrainedSuperResolution,"
             "bool superResolution )" );

}


template < class S >
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf( 
                                  const gkg::OrientationSet* orientationSet,
                                  const gkg::Volume< float >& t2,
                                  const gkg::Volume< float >& dw,
                                  const gkg::Vector3d< S >& site,
                                  const gkg::Matrix& reconstructionMatrix,
                                  const gkg::Matrix& Bplus )
                                    : gkg::SHFiberOdf( orientationSet )
{

  try
  {

    gkg::Vector normalizedSignal;
    this->reconstructUnregularized( orientationSet,
                                    t2,
                                    dw,
                                    site,
                                    reconstructionMatrix,
                                    Bplus,
                                    normalizedSignal );

  }
  GKG_CATCH( "template < class S > "
             "gkg::SphericalDeconvolutionFiberOdf::"
             "SphericalDeconvolutionFiberOdf( "
             "const gkg::OrientationSet* orientationSet,"
             "const gkg::Volume< float >& t2,"
             "const gkg::Volume< float >& dw,"
             "const gkg::Vector3d< int32_t >& site,"
             "const gkg::Matrix& reconstructionMatrix"
             "const gkg::Matrix& Bplus )" );

}


template < class S >
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf( 
                                  const gkg::OrientationSet* orientationSet,
                                  const gkg::Volume< float >& t2,
                                  const gkg::Volume< float >& dw,
                                  const gkg::Vector3d< S >& site,
                                  const gkg::Matrix& reconstructionMatrix,
                                  const gkg::Matrix& B,
                                  const gkg::Matrix& Bplus,
                                  const gkg::Matrix& regularizationMatrix,
                                  const gkg::Matrix& convolutionMatrix,
                                  bool superResolution )
                                    : gkg::SHFiberOdf( orientationSet )
{

  try
  {

    gkg::Vector normalizedSignal;
    this->reconstructUnregularized( orientationSet,
    				    t2,
    				    dw,
                                    site,
                                    reconstructionMatrix,
                                    Bplus,
                                    normalizedSignal );

    applyConstrainedSuperResolutionRegularization( normalizedSignal,
                                                   B,
                                                   regularizationMatrix,
                                                   convolutionMatrix,
                                                   superResolution );

  }
  GKG_CATCH( "template < class S > "
             "gkg::SphericalDeconvolutionFiberOdf::"
             "SphericalDeconvolutionFiberOdf(  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& B, "
             "const gkg::Matrix& Bplus, "
             "const gkg::Matrix& regularizationMatrix, "
             "const gkg::Matrix& convolutionMatrix, "
             "bool superResolution )" );

}


gkg::SphericalDeconvolutionFiberOdf::~SphericalDeconvolutionFiberOdf()
{
}


const gkg::Vector& 
gkg::SphericalDeconvolutionFiberOdf::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::SphericalDeconvolutionOdf::"
	     "getDwSphericalHarmonicsCoefficients() const" );

}



void gkg::SphericalDeconvolutionFiberOdf::getReconstructionMatrix(
                                    const gkg::HeaderedObject& headeredObject,
                                    const gkg::DiffusionKernel& diffusionKernel,
                                    int32_t maximumSphericalHarmonicOrder,
                                    const gkg::Vector& filteringCoefficients,
                                    double bValue,
                                    gkg::Matrix& reconstructionMatrix,
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
    B.reallocate( M, N );
    int32_t n, m;
    gkg::Vector3d< float > orientation;
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;
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

    // building Bplus matrix with size N x M
    // Bplus = (B^T B )^(-1) B^T
    Bplus.reallocate( N, M );
    factory->getMoorePenrosePseudoInverse( B, Bplus );

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

    // need bValue in s/m^2
    gkg::Vector signal = generatingTensor.getRotatedDiffusionWeightedSignal(
                                                      *_orientationSet,
                                                      rotationMatrix,
                                                      bValue,
                                                      1.0 );

    gkg::Vector sphericalHarmonicSynthetic1FiberSignal = 
                                                Bplus.getComposition( signal );

    gkg::Vector diracFunction( N );
    gkg::Vector nonZeroSphericalHarmonicSynthetic1FiberSignal( 
                                      maximumSphericalHarmonicOrder / 2 + 1 );
    gkg::Vector nonZeroDiracFunction( maximumSphericalHarmonicOrder / 2 + 1 );

    int32_t i = 0;
    int32_t l = 0;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      diracFunction( n ) = factory->getDiracCoefficient( n, 0, 0 );
      if ( m == 0 )
      {

        nonZeroSphericalHarmonicSynthetic1FiberSignal( i ) = 
                                   sphericalHarmonicSynthetic1FiberSignal( n );
        nonZeroDiracFunction( i ) = diracFunction( n );
        ++ i;

      }

    }

    gkg::Matrix R( N, N );
    gkg::Matrix Rinverse( N, N );
    R.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );

      // R(n n) = syntheticSignal(l, m=0) / Dirac(l,m)
      R( n, n ) = filteringCoefficients( l / 2 ) *
                  ( nonZeroSphericalHarmonicSynthetic1FiberSignal( l / 2 ) /
                    nonZeroDiracFunction( l / 2 ) );

      if( R( n, n) != 0 )
      {

        Rinverse( n, n ) = 1 / R( n , n);

      }

    }

    // building reconstruction matrix , deconvolution with Rinverse
    reconstructionMatrix = Rinverse.getComposition( Bplus );

  }
  GKG_CATCH( "void gkg::SphericalDeconvolutionFiberOdf::"
             "getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::DiffusionKernel& diffusionKernel,"
             "int32_t maximumSphericalHarmonicOrder,"
             "const gkg::Vector& filteringCoefficients,"
             "double bValue,"
             "gkg::Matrix& reconstructionMatrix,"
             "gkg::Matrix& Bplus,"
             "gkg::Matrix& BMatrix )" );

}


template < class S >
bool gkg::SphericalDeconvolutionFiberOdf::reconstructUnregularized(
                                    const gkg::OrientationSet* orientationSet,
                                    const gkg::Volume< float >& t2,
                                    const gkg::Volume< float >& dw,
                                    const gkg::Vector3d< S >& site,
                                    const gkg::Matrix& reconstructionMatrix,
                                    const gkg::Matrix& Bplus,
                                    gkg::Vector& normalizedSignal )
{

  try
  {

    bool isOk = false;
    if ( orientationSet )
    {

      // computing average T2 value and normalized signal vector
      float averageT2 = 0.0;
      
      isOk = this->computeNormalizedSignal( t2,
                                            dw,
                                            site,
                                            averageT2,
                                            normalizedSignal );

      // resizing ODF vector to SH coefficient count
      int32_t shCoefficientCount = Bplus.getSize1();
      _values.resize( shCoefficientCount );

      if ( !isOk )
      {

        // setting DW spherical harmonics coefficient(s) to nul vector
        _dwSphericalHarmonicsCoefficients.reallocate( shCoefficientCount );
        _dwSphericalHarmonicsCoefficients.fill( 0.0 );

        // setting ODF SH coefficients to nul
        int32_t s;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          _values[ s ] = 0.0f;

        }

      }
      else
      {


        // processing DW spherical harmonics coefficient(s)
        _dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

        // processing f-ODF (ie FOD) spherical harmonics coefficient(s)
        gkg::Vector fOdfSphericalHarmonicsCoefficients =
                        reconstructionMatrix.getComposition( normalizedSignal );

        // copying SH coefficients
        int32_t s;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          _values[ s ] = ( float )fOdfSphericalHarmonicsCoefficients( s );

        }

      }

    }

    return isOk;

  }
  GKG_CATCH( "template < class S > "
             "void gkg::SphericalDeconvolutionFiberOdf::"
             "reconstructUnregularized( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus, "
             "gkg::Vector& normalizedSignal )" );

}


template 
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const DiffusionKernel& kernel,
                             int32_t maximumSphericalHarmonicOrder,
                             const Vector& filteringCoefficients,
                             bool constrainedSphericalDeconvolution,
                             bool superResolution );

template 
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const DiffusionKernel& kernel,
                             int32_t maximumSphericalHarmonicOrder,
                             const Vector& filteringCoefficients,
                             bool constrainedSphericalDeconvolution,
                             bool superResolution );


template
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& B,
                             const gkg::Matrix& Bplus,
                             const Matrix& regularizationMatrix,
                             const Matrix& convolutionMatrix,
                             bool superResolution );

template
gkg::SphericalDeconvolutionFiberOdf::SphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& B,
                             const gkg::Matrix& Bplus,
                             const Matrix& regularizationMatrix,
                             const Matrix& convolutionMatrix,
                             bool superResolution );

template
bool gkg::SphericalDeconvolutionFiberOdf::reconstructUnregularized(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus,
                             gkg::Vector& normalizedSignal );

template
bool gkg::SphericalDeconvolutionFiberOdf::reconstructUnregularized(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus,
                             gkg::Vector& normalizedSignal );
