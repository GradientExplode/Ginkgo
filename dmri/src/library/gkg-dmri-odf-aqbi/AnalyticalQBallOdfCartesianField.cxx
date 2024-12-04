#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianField.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdf.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::AnalyticalQBallOdfCartesianField::AnalyticalQBallOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        outputOrientationSet )
{

  try
  {

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 0 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 1 ];
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }
    _laplacianSharpening = ( float )specificScalarParameters[ 2 ];
    if ( _laplacianSharpening < 0 )
    {

      throw std::runtime_error( "Laplacian sharpening must be positive" );

    }

    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::AnalyticalQBallOdf >::getName() );

    this->_functor = new gkg::AnalyticalQBallOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::AnalyticalQBallOdfCartesianField::"
             "AnalyticalQBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AnalyticalQBallOdfCartesianField::AnalyticalQBallOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        superResolution,
                                                        outputOrientationSet )
{

  try
  {

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 0 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 1 ];
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }
    _laplacianSharpening = ( float )specificScalarParameters[ 2 ];
    if ( _laplacianSharpening < 0 )
    {

      throw std::runtime_error( "Laplacian sharpening must be positive" );

    }

    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::AnalyticalQBallOdf >::getName() );

    this->_functor = new gkg::AnalyticalQBallOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::AnalyticalQBallOdfCartesianField::"
             "AnalyticalQBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AnalyticalQBallOdfCartesianField::AnalyticalQBallOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        outputOrientationSet )
{

  try
  {

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 0 ]
                                                  + 0.5 );
    if ( _maximumSphericalHarmonicOrder < 0 )
    {

      throw std::runtime_error(
                          "maximum spherical harmonic order must be positive" );

    }
    if ( _maximumSphericalHarmonicOrder % 2 )
    {

      throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

    }

    _regularizationLCurveFactor = ( float )specificScalarParameters[ 1 ];
    if ( _regularizationLCurveFactor < 0 )
    {

      throw std::runtime_error( 
                             "regularization L-curve factor must be positive" );

    }
    _laplacianSharpening = ( float )specificScalarParameters[ 2 ];
    if ( _laplacianSharpening < 0 )
    {

      throw std::runtime_error( "Laplacian sharpening must be positive" );

    }

    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::AnalyticalQBallOdf >::getName() );

    this->_functor = new gkg::AnalyticalQBallOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::AnalyticalQBallOdfCartesianField::"
             "AnalyticalQBallOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AnalyticalQBallOdfCartesianField::~AnalyticalQBallOdfCartesianField()
{
}



int32_t 
gkg::AnalyticalQBallOdfCartesianField::getMaximumSphericalHarmonicOrder() const
{

  return _maximumSphericalHarmonicOrder;

}


int32_t gkg::AnalyticalQBallOdfCartesianField::getSphericalHarmonicCount() const
{

  return ( ( _maximumSphericalHarmonicOrder + 1 ) *
           ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

}


float 
gkg::AnalyticalQBallOdfCartesianField::getRegularizationLCurveFactor() const
{

  return _regularizationLCurveFactor;

}


float gkg::AnalyticalQBallOdfCartesianField::getLaplacianSharpening() const
{

  return _laplacianSharpening;

}


const gkg::Matrix& 
gkg::AnalyticalQBallOdfCartesianField::getReconstructionMatrix() const
{

  return _reconstructionMatrix;

}


const gkg::Matrix& gkg::AnalyticalQBallOdfCartesianField::getBplus() const
{

  return _Bplus;

}


void gkg::AnalyticalQBallOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 3 );
      specificScalarParameters[ 0 ] = 4;
      specificScalarParameters[ 1 ] = 0.006;
      specificScalarParameters[ 2 ] = 0.0;

    }
    else
    {

      if ( specificScalarParameters.size() != 3U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

    }
    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error( "no specific string parameter" );

    }

  }
  GKG_CATCH( "void gkg::AnalyticalQBallOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void 
gkg::AnalyticalQBallOdfCartesianField::getReconstructionMatrix( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "building reconstruction matrix" << std::flush;

    }

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = ( ( _maximumSphericalHarmonicOrder + 1 ) *
                  ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

    // measure count
    int32_t M = _inputOrientationSet.getCount();

    // building matrix of spherical harmonics on input orientation(s)
    gkg::Matrix B( M, N );
    int32_t n, m;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( m = 0; m < M; m++ )
    {

      orientation = _inputOrientationSet.getOrientation( m );
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

    // building Laplace-Beltrami smoothness matrix for regulrization
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
    _Bplus.reallocate( N, M );
    factory->getThikonovPseudoInverse( B, L,
                                       ( double )_regularizationLCurveFactor,
                                       _Bplus );

    // building Funk-Hecke matrix P
    gkg::Matrix P( N, N );
    P.setZero();
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      P( n, n ) = 2 * M_PI * ( factory->getLegendrePolynomial( l, 0.0 ) / 
                               factory->getLegendrePolynomial( l, 1.0 ) );

    }

    if ( _laplacianSharpening > 0 )
    {

      // building Laplace-Beltrami sharpening matrix
      gkg::Matrix Sharpening( N, N );
      Sharpening.setZero();
      for ( n = 0; n < N; n++ )
      {

        factory->getSymmetricalSphericalHarmonicLM( n, l, m );
        Sharpening( n, n ) = 1 + _laplacianSharpening * l * ( l + 1 );

      }

      // building reconstruction matrix PBplus
      _reconstructionMatrix = P.getComposition( Sharpening ).getComposition(
                                                                       _Bplus );

    }
    else
    {

      // building reconstruction matrix PBplus
      _reconstructionMatrix = P.getComposition( _Bplus );

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::AnalyticalQBallOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::AnalyticalQBallOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::AnalyticalQBallOdf analyticalQBallOdf( &_outputOrientationSet,
                                                *_t2, *_dw,
                                                originSite,
                                                _reconstructionMatrix,
                                                _Bplus );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              analyticalQBallOdf );

    }
    else
    {

      return analyticalQBallOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AnalyticalQBallOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::AnalyticalQBallOdfCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::AnalyticalQBallOdf analyticalQBallOdf( &_outputOrientationSet,
                                                *_t2, *_dw,
                                                originSite,
                                                _reconstructionMatrix,
                                                _Bplus );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              analyticalQBallOdf );

    }
    else
    {

      return analyticalQBallOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AnalyticalQBallOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
