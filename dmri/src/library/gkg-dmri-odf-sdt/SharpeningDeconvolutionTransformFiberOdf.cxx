#include <gkg-dmri-odf-sdt/SharpeningDeconvolutionTransformFiberOdf.h>
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
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                      SharpeningDeconvolutionTransformFiberOdf(
                            const gkg::OrientationSet* orientationSet,
                            const gkg::Volume< float >& t2,
                            const gkg::Volume< float >& dw,
                            const gkg::Vector3d< S >& site,
                            const gkg::DiffusionKernel& diffusionKernel,
                            int32_t maximumSphericalHarmonicOrder,
                            float regularizationLCurveFactor,
                            bool constrainedSphericalDeconvolution,
                            bool superResolution )
                                              : gkg::SHFiberOdf(
                                                               orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrices
      gkg::Matrix reconstructionMatrix;
      gkg::Matrix B;
      gkg::Matrix Bplus;

      getReconstructionMatrix( dw,
                               diffusionKernel,
                               maximumSphericalHarmonicOrder,
                               regularizationLCurveFactor,
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
             "gkg::SharpeningDeconvolutionTransformFiberOdf::"
             "SharpeningDeconvolutionTransformFiberOdf(  "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::DiffusionKernel& diffusionKernel, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "bool constrainedSuperResolution"
             "bool superResolution )" );

}


template < class S >
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                      SharpeningDeconvolutionTransformFiberOdf(
                            const gkg::OrientationSet* orientationSet,
                            const gkg::Volume< float >& t2,
                            const gkg::Volume< float >& dw,
                            const gkg::Vector3d< S >& site,
                            const gkg::Matrix& reconstructionMatrix,
                            const gkg::Matrix& Bplus )
                                              : gkg::SHFiberOdf(
                                                               orientationSet )
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
             "gkg::SharpeningDeconvolutionTransformFiberOdf::"
             "SharpeningDeconvolutionTransformFiberOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus)" );

}


template < class S >
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                      SharpeningDeconvolutionTransformFiberOdf(
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
                                              : gkg::SHFiberOdf(
                                                                orientationSet )
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
             "gkg::SharpeningDeconvolutionTransformFiberOdf::"
             "SharpeningDeconvolutionTransformFiberOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& B,"
             "const gkg::Matrix& Bplus,"
             "const gkg::Matrix& regularizationMatrix,"
             "const gkg::Matrix& convolutionMatrix,"
             "bool superResolution )" );

}


gkg::SharpeningDeconvolutionTransformFiberOdf::
                                    ~SharpeningDeconvolutionTransformFiberOdf()
{
}


const gkg::Vector& gkg::SharpeningDeconvolutionTransformFiberOdf
                                   ::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::SharpeningDeconvolutionTransformOdf::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


void gkg::SharpeningDeconvolutionTransformFiberOdf::getReconstructionMatrix(
                                   const gkg::HeaderedObject& headeredObject,
                                   const gkg::DiffusionKernel& diffusionKernel,
                                   int32_t maximumSphericalHarmonicOrder,
                                   float regularizationLCurveFactor,
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
    double radial;
    double theta;
    double phi;
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

    // building Funk-Hecke matrix P
    gkg::Matrix P( N, N );
    P.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      P( n, n ) = 2 * M_PI * ( factory->getLegendrePolynomial( l, 0.0 ) );

    }


    // building Sharpening Deconvolution Transform (SDT) matrix
    gkg::Vector eigenValues = diffusionKernel.getTensor().getEigenValues();
    gkg::Matrix SDT( N, N );
    SDT.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      SDT( n, n ) = factory->getSharpeningDeconvolutionTransformCoefficient(
                                                         l,
                                                         eigenValues( 0 ),
                                                         eigenValues( 1 ) );

    }

    // building reconstruction matrix PBplus
    reconstructionMatrix = P.getComposition( SDT ).getComposition( Bplus );

  }
  GKG_CATCH( "void gkg::SharpeningDeconvolutionTransformFiberOdf::"
             "getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::DiffusionKernel& diffusionKernel, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "gkg::Matrix& reconstructionMatrix, "
             "gkg::Matrix& Bplus )" );

}


template < class S >
bool gkg::SharpeningDeconvolutionTransformFiberOdf::reconstructUnregularized(
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
             "void gkg::SharpeningDeconvolutionTransformFiberOdf::"
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
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const DiffusionKernel& kernel,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             bool constrainedSuperResolution,
                             bool superResolution );

template 
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const DiffusionKernel& kernel,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             bool constrainedSuperResolution,
                             bool superResolution );


template
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
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
gkg::SharpeningDeconvolutionTransformFiberOdf::
                                       SharpeningDeconvolutionTransformFiberOdf(
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
bool gkg::SharpeningDeconvolutionTransformFiberOdf::reconstructUnregularized(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus,
                             gkg::Vector& normalizedSignal );

template
bool gkg::SharpeningDeconvolutionTransformFiberOdf::reconstructUnregularized(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus,
                             gkg::Vector& normalizedSignal );



