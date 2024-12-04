#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class S >
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                                      const gkg::OrientationSet* orientationSet,
                                      const gkg::Volume< float >& t2,
                                      const gkg::Volume< float >& dw,
                                      const gkg::Vector3d< S >& site,
                                      int32_t maximumSphericalHarmonicOrder,
                                      float regularizationLCurveFactor,
                                      float laplacianSharpening )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      gkg::Matrix Bplus;
      getReconstructionMatrix( dw,
                               maximumSphericalHarmonicOrder,
                               regularizationLCurveFactor,
                               laplacianSharpening,
                               reconstructionMatrix,
                               Bplus );

      this->reconstruct( orientationSet,
                         t2,
                         dw,
                         site,
                         reconstructionMatrix,
                         Bplus );

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "float laplacianSharpening )" );

}


template < class S >
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< S >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus )
                : gkg::OrientationDistributionFunction(
                    gkg::OrientationDistributionFunction::SphericalHarmonics,
                    orientationSet )
{

  try
  {

    this->reconstruct( orientationSet,
                       t2,
                       dw,
                       site,
                       reconstructionMatrix,
                       Bplus );

  }
  GKG_CATCH( "template < class S > "
             "gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus )" );

}


gkg::SolidAngleAnalyticalQBallOdf::~SolidAngleAnalyticalQBallOdf()
{
}


const gkg::Vector& 
gkg::SolidAngleAnalyticalQBallOdf::getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::SolidAngleAnalyticalQBallOdf::"
	     "getDwSphericalHarmonicsCoefficients() "
	     "const" );

}


void gkg::SolidAngleAnalyticalQBallOdf::getReconstructionMatrix(
                                      const gkg::HeaderedObject& headeredObject,
                                      int32_t maximumSphericalHarmonicOrder,
                                      float regularizationLCurveFactor,
                                      float laplacianSharpening,
                                      gkg::Matrix& reconstructionMatrix,
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
    gkg::Matrix B( M, N );
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

    // building Laplace-Beltrami and square Laplace-Beltrami regularization
    // matrices
    gkg::Matrix L( N, N );
    L.setZero();
    gkg::Matrix L2( N, N );
    L2.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian;
      L2( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L2 )^(-1) B^T
    Bplus.reallocate( N, M );
    factory->getThikonovPseudoInverse( B, L2,
                                       ( double )regularizationLCurveFactor,
                                       Bplus );

    // building Funk-Hecke matrix P
    gkg::Matrix P( N, N );
    P.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      P( n, n ) = 2 * M_PI * factory->getLegendrePolynomial( l, 0.0 );

    }

    if ( laplacianSharpening > 0 )
    {

      // building Laplace-Beltrami sharpening matrix
      gkg::Matrix Sharpening( N, N );
      Sharpening.setZero();
      for ( n = 0; n < N; n++ )
      {

        factory->getSymmetricalSphericalHarmonicLM( n, l, m );
        Sharpening( n, n ) = 1 + laplacianSharpening * l * ( l + 1 );

      }

      // building reconstruction matrix PBplus
      reconstructionMatrix = P.getComposition( Sharpening ).
                                  getComposition( L ).getComposition( Bplus );

    }
    else
    {

      // building reconstruction matrix PBplus
      reconstructionMatrix = P.getComposition( L ).getComposition( Bplus );

    }

  }
  GKG_CATCH( "void gkg::SolidAngleAnalyticalQBallOdf::getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "int32_t maximumSphericalHarmonicOrder, "
             "float regularizationLCurveFactor, "
             "gkg::Matrix& reconstructionMatrix, "
             "gkg::Matrix& Bplus )" );

}


template < class S >
void gkg::SolidAngleAnalyticalQBallOdf::reconstruct(
                                      const gkg::OrientationSet* orientationSet,
                                      const gkg::Volume< float >& t2,
                                      const gkg::Volume< float >& dw,
                                      const gkg::Vector3d< S >& site,
                                      const gkg::Matrix& reconstructionMatrix,
                                      const gkg::Matrix& Bplus )
{

  try
  {

    if ( orientationSet )
    {

      // computing average T2 value and normalized signal vector
      float averageT2 = 0.0;
      gkg::Vector normalizedSignal;
      bool isOk = this->computeNormalizedSignal( t2,
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
        _values[ 0 ] = 1 / ( 2.0 * std::sqrt( M_PI ) );
        for ( s = 1; s < shCoefficientCount; s++ )
        {

          _values[ s ] = 0.0f;

        }

      }
      else
      {

        // building double log normalized signal vector
        int32_t orientationCount = dw.getSizeT();
        gkg::Vector doubleLogNormalizedSignal( orientationCount );
        int32_t t = 0;
        for ( t = 0; t < orientationCount; t++ )
        {

          doubleLogNormalizedSignal( t ) =
                                 std::log( -std::log( normalizedSignal( t ) ) );

        }

        // processing DW spherical harmonics coefficient(s)
        _dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );
        // processing ODF spherical harmonics coefficient(s)
        gkg::Vector odfSphericalHarmonicsCoefficients =
               reconstructionMatrix.getComposition( doubleLogNormalizedSignal );

        // copying SH coefficients
        int32_t s;
        float scaleFactor = ( 1.0 / ( 16.0 * M_PI * M_PI ) );
        _values[ 0 ] = 1 / ( 2.0 * std::sqrt( M_PI ) );
        for ( s = 1; s < shCoefficientCount; s++ )
        {

          _values[ s ] = scaleFactor * 
                         ( float )odfSphericalHarmonicsCoefficients( s );

        }

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "void gkg::SolidAngleAnalyticalQBallOdf::reconstruct( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& reconstructionMatrix, "
             "const gkg::Matrix& Bplus )" );

}


template 
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             float laplacianSharpening );

template 
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             float laplacianSharpening );


template
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
gkg::SolidAngleAnalyticalQBallOdf::SolidAngleAnalyticalQBallOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
void gkg::SolidAngleAnalyticalQBallOdf::reconstruct(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );

template
void gkg::SolidAngleAnalyticalQBallOdf::reconstruct(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             const gkg::Matrix& reconstructionMatrix,
                             const gkg::Matrix& Bplus );
