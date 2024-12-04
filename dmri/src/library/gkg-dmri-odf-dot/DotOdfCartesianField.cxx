#include <gkg-dmri-odf-dot/DotOdfCartesianField.h>
#include <gkg-dmri-odf-dot/DotOdf.h>
#include <gkg-dmri-odf-dot/DotOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::DotOdfCartesianField::DotOdfCartesianField(
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
    _effectiveDiffusionTime = (float)(specificScalarParameters[ 1 ]*pow(10.0,-3) );
    _R0 = ( float ) ( specificScalarParameters[ 2 ]*pow(10.0,-3) );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::DotOdf >::getName() );

    this->_functor = new gkg::DotOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );
    
  }
  GKG_CATCH( "gkg::DotOdfCartesianField::DotOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
	     "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DotOdfCartesianField::DotOdfCartesianField(
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
    _effectiveDiffusionTime = (float)(specificScalarParameters[ 1 ]*pow(10.0,-3));
    _R0 = ( float ) ( specificScalarParameters[ 2 ]*pow(10.0,-3) );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );
    
    _items.getHeader().addAttribute( "odf_type",
                                    gkg::TypeOf< gkg::DotOdf >::getName() );

    this->_functor = new gkg::DotOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::DotOdfCartesianField::DotOdfCartesianField( "
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


gkg::DotOdfCartesianField::DotOdfCartesianField(
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
    _effectiveDiffusionTime =
                   ( float )( specificScalarParameters[ 1 ] * pow( 10.0, -3 ) );
    _R0 = ( float )( specificScalarParameters[ 2 ] * pow( 10.0, -3 ) );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );


    _items.getHeader().addAttribute( "odf_type",
                                    gkg::TypeOf< gkg::DotOdf >::getName() );

    this->_functor = new gkg::DotOdfCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::DotOdfCartesianField::DotOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DotOdfCartesianField::~DotOdfCartesianField()
{
}

int32_t gkg::DotOdfCartesianField::getMaximumSphericalHarmonicOrder() const
{

  return _maximumSphericalHarmonicOrder;

}

int32_t gkg::DotOdfCartesianField::getSphericalHarmonicCount() const
{

  return (_maximumSphericalHarmonicOrder+1)*(_maximumSphericalHarmonicOrder+2)/2;

}

float gkg::DotOdfCartesianField::getEffectiveDiffusionTime() const
{

  return _R0;

}

float gkg::DotOdfCartesianField::getR0() const
{

  return _R0;

}

bool gkg::DotOdfCartesianField::getMarginalODF() const
{

  return _marginalODF;

}


void gkg::DotOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {
      
      specificScalarParameters.resize( 4 );

      specificScalarParameters[ 0 ] = 4;                  
                                                   //  order 4 by default
      specificScalarParameters[ 1 ] = 25 * std::pow( 10.0, -3 );
                                                   // 25 ms by default
      specificScalarParameters[ 2 ] = 12 * std::pow( 10.0, -3 );
                                                   // 12 um by default
      specificScalarParameters[ 3 ] = 1;   
                                                   // ODF computation by default

    }
    else
    {

      if ( specificScalarParameters.size() != 4U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }
      
      int32_t maxSHorder = (int32_t)specificScalarParameters[ 0 ];
      
      if( maxSHorder % 2 ) 
      {
	
	throw std::runtime_error( 
                              "maximum spherical harmonic order must be even" );

      }

      if ( maxSHorder < 0 )
      {

	throw std::runtime_error(
			  "maximum spherical harmonic order must be positive" );

      }

      if( maxSHorder > 8 ) 
      {
	
	std::cerr << "!Warning!: DOT only tabulated and tested up to order 8"
		  << ": !Warning!\n";
	
      }
      
      if( specificScalarParameters [ 2 ] < 0 ) 
      {
	
	throw std::runtime_error( "invalid scalar parameter 'R0'" );

      }

    }

    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error( "no specific string parameter" );

    }

  }
  GKG_CATCH( "void gkg::DotOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::DotOdfCartesianField::getReconstructionMatrix( bool verbose )
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
    _reconstructionMatrix.reallocate( M, N );
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

        _reconstructionMatrix( m, n ) = 
	                 factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B)^(-1) B^T
    _Bplus.reallocate( N, M );
    factory->getMoorePenrosePseudoInverse( _reconstructionMatrix, _Bplus );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::DotOdfCartesianField::getReconstructionMatrix( "
             "bool verbose )" );
}


gkg::OrientationDistributionFunction 
gkg::DotOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DotOdf dotOdf( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_effectiveDiffusionTime, _R0, _marginalODF,
			_reconstructionMatrix,
			_Bplus );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dotOdf );

    }
    else
    {

      return dotOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DotOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::DotOdfCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DotOdf dotOdf( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_effectiveDiffusionTime, _R0, _marginalODF,
			_reconstructionMatrix,
			_Bplus );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dotOdf );

    }
    else
    {

      return dotOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DotOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}



