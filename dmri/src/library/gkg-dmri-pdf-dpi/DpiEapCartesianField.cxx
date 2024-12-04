#include <gkg-dmri-pdf-dpi/DpiEapCartesianField.h>
#include <gkg-dmri-pdf-dpi/DpiEap.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>

gkg::DpiEapCartesianField::DpiEapCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::EapCartesianField( t2, dw, mask,
                                                        outputOrientationSet )
{

  try
  {

    _numberOfShells = ( int32_t )( specificScalarParameters[ 0 ] );
    _numberOfOrientationsPerShell =
                                   ( int32_t )( specificScalarParameters[ 1 ] );

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 2 ]
                                                  + 0.5 );

    if ( _numberOfShells < 2 ) 
    {

      throw std::runtime_error(
			      "Must have at least two shells of measurements" );

    }

    if ( _numberOfOrientationsPerShell*_numberOfShells < 
	 ( _maximumSphericalHarmonicOrder + 1 )*
	 (_maximumSphericalHarmonicOrder + 2) ) 
    {
      
      char message[300];
      std::string output = "Not enough diffusion measurements per shell for "
                           "the number of unknown coefficients : "; 
      std::string output2 = " required by a maximum spherical harmonic order "
                            "of "; 
      sprintf( message, "%s%d%s%d", output.c_str(),
	      ( _maximumSphericalHarmonicOrder + 1 )*
	      (_maximumSphericalHarmonicOrder + 2),
	      output2.c_str(), _maximumSphericalHarmonicOrder );


      throw std::runtime_error( message );

    }

    
    int32_t totalNumberOfbValues = this->getBValues().size();

    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
		 "Error in dw minf attributes : number of bValues "
                 "and number of diffusion gradient orientations not the same" );

    }
    
    if ( _numberOfShells > 
	 ( totalNumberOfbValues / _numberOfOrientationsPerShell ) )
    {
      
      throw std::runtime_error(
			       "Not enough shell acquisition for the required "
                               "number of shells in the DPI reconstruction" );

    }

    if ( ( _inputOrientationSet.getCount() %
           _numberOfOrientationsPerShell ) != 0 )
    {

      throw std::runtime_error(
		        "Error in  number of shells or number of orientations "
                        "per shells with respect to DW data inputed" );

    }


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

    _items.getHeader().addAttribute( "eap_type",
                                     gkg::TypeOf< gkg::DpiEap >::getName() );

    this->_functor = new gkg::DpiEapCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::DpiEapCartesianField::DpiEapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DpiEapCartesianField::DpiEapCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::EapCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        superResolution,
                                                        outputOrientationSet )
{

  try
  {

    _numberOfShells = ( int32_t )( specificScalarParameters[ 0 ] );
    _numberOfOrientationsPerShell = 
                                   ( int32_t )( specificScalarParameters[ 1 ] );

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 2 ]
                                                  + 0.5 );
    if ( _numberOfShells < 2 ) 
    {

      throw std::runtime_error(
			      "Must have at least two shells of measurements" );

    }

    if ( _numberOfOrientationsPerShell*_numberOfShells < 
	 ( _maximumSphericalHarmonicOrder + 1 )*
	 (_maximumSphericalHarmonicOrder + 2) ) 
    {
      
      char message[300];
      std::string output = "Not enough diffusion measurements per shell for "
                           "the number of unknown coefficients : "; 
      std::string output2 = " required by a maximum spherical harmonic order "
                            "of "; 
      sprintf( message, "%s%d%s%d", output.c_str(),
	      ( _maximumSphericalHarmonicOrder + 1 )*
	      (_maximumSphericalHarmonicOrder + 2),
	      output2.c_str(), _maximumSphericalHarmonicOrder );


      throw std::runtime_error( message );

    }

    
    int32_t totalNumberOfbValues = this->getBValues().size();

    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
		 "Error in dw minf attributes : number of bValues "
                 "and number of diffusion gradient orientations not the same" );

    }
    
    if ( _numberOfShells > 
	 ( totalNumberOfbValues / _numberOfOrientationsPerShell ) )
    {
      
      throw std::runtime_error(
			       "Not enough shell acquisition for the required "
                               "number of shells in the DPI reconstruction" );

    }

    if ( ( _inputOrientationSet.getCount() %
           _numberOfOrientationsPerShell ) != 0 )
    {

      throw std::runtime_error(
		       "Error in  number of shells or number of orientations "
                       "per shells with respect to DW data inputed" );

    }


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

    _items.getHeader().addAttribute( "eap_type",
                                    gkg::TypeOf< gkg::DpiEap >::getName() );

    this->_functor = new gkg::DpiEapCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::DpiEapCartesianField::DpiEapCartesianField( "
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


gkg::DpiEapCartesianField::DpiEapCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::EapCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        outputOrientationSet )
{

  try
  {
    
    _numberOfShells = ( int32_t )( specificScalarParameters[ 0 ] );
    _numberOfOrientationsPerShell =
                                   ( int32_t )( specificScalarParameters[ 1 ] );

    _maximumSphericalHarmonicOrder = ( int32_t )( specificScalarParameters[ 2 ]
                                                  + 0.5 );

    if ( _numberOfShells < 2 ) 
    {

      throw std::runtime_error(
			      "Must have at least two shells of measurements" );

    }

    if ( _numberOfOrientationsPerShell*_numberOfShells < 
	 ( _maximumSphericalHarmonicOrder + 1 )*
	 (_maximumSphericalHarmonicOrder + 2) ) 
    {

      char message[300];
      std::string output = "Not enough diffusion measurements per shell for "
                           "the number of unknown coefficients : "; 
      std::string output2 = " required by a maximum spherical harmonic order "
                            "of "; 
      sprintf( message, "%s%d%s%d", output.c_str(),
	      ( _maximumSphericalHarmonicOrder + 1 )*
	      (_maximumSphericalHarmonicOrder + 2),
	      output2.c_str(), _maximumSphericalHarmonicOrder );

      throw std::runtime_error( message );

    }

    
    int32_t totalNumberOfbValues = this->getBValues().size();

    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
		 "Error in dw minf attributes : number of bValues "
                 "and number of diffusion gradient orientations not the same" );

    }
    
    if ( _numberOfShells > 
	 ( totalNumberOfbValues / _numberOfOrientationsPerShell ) )
    {
      
      throw std::runtime_error(
			       "Not enough shell acquisition for the required "
                               "number of shells in the DPI reconstruction" );

    }

    if ( ( _inputOrientationSet.getCount() % 
           _numberOfOrientationsPerShell ) != 0 )
    {

      throw std::runtime_error(
		       "Error in  number of shells or number of orientations "
                       "per shells with respect to DW data inputed" );

    }

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

    _items.getHeader().addAttribute( "eap_type",
                                    gkg::TypeOf< gkg::DpiEap >::getName() );

    this->_functor = new gkg::DpiEapCartesianFieldFunctor( *this );

    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::DpiEapCartesianField::DpiEapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DpiEapCartesianField::~DpiEapCartesianField()
{
}



int32_t gkg::DpiEapCartesianField::getMaximumSphericalHarmonicOrder() const
{

  return _maximumSphericalHarmonicOrder;

}


int32_t gkg::DpiEapCartesianField::getDataSphericalHarmonicCount() const
{

  return ( _maximumSphericalHarmonicOrder + 1 ) *
         ( _maximumSphericalHarmonicOrder + 2 ) ;

}

int32_t gkg::DpiEapCartesianField::getEapSphericalHarmonicCount() const
{

  return ( ( _maximumSphericalHarmonicOrder + 1 ) *
           ( _maximumSphericalHarmonicOrder + 2 ) ) / 2;

}

int32_t gkg::DpiEapCartesianField::getNumberOfShells() const
{

  return _numberOfShells;

}

int32_t gkg::DpiEapCartesianField::getTotalNumberOfShells() const
{

  return _inputOrientationSet.getCount() / _numberOfOrientationsPerShell;

}

int32_t gkg::DpiEapCartesianField::getNumberOfOrientationsPerShell() const
{

  return _numberOfOrientationsPerShell;

}


int32_t gkg::DpiEapCartesianField::getTotalNumberOfOrientations() const
{

  return _inputOrientationSet.getCount();

}


const gkg::Matrix& 
gkg::DpiEapCartesianField::getReconstructionMatrix() const
{

  return _reconstructionMatrix;

}


void gkg::DpiEapCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {

      specificScalarParameters.resize( 3 );
      specificScalarParameters[ 0 ] = 0;
      specificScalarParameters[ 1 ] = 0;
      specificScalarParameters[ 2 ] = 4;

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
  GKG_CATCH( "void gkg::DpiEapCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::DpiEapCartesianField::getReconstructionMatrix( bool verbose )
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
    int32_t M = _numberOfOrientationsPerShell;

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

    // Laplacian smoothing matrix 
    gkg::Matrix L( 2*N, 2*N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    // fill in first half only
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;
      L( N + n, N + n ) = sphericalLaplacian * sphericalLaplacian;
      
    }

    // compute irregular and regular solid harmonics exponents for each shell
    // I1 = 1/ q1^{l(j) + 1}, I2 = 1/ q2^{l(j) + 1}, etc...
    // R1 = q1^{l(j)}, R2 = q2^{l(j)}, etc...
    gkg::Matrix IrregularSolidHarmonicCoefficients[ _numberOfShells ];
    gkg::Matrix RegularSolidHarmonicCoefficients[ _numberOfShells ];

    int32_t s, j;
    //std::vector < float > qValues;
    if( verbose ) 
    {
	
      std::cout << "\nList of q/b-Values [";
	
    }
    for( s = 0; s < _numberOfShells; s++)
    {
      
      // transform bValue into qValue
      // first divide by 1000 and than take square root
      // we do not take effective diffusion time and 4 pi in consideration
      // we simply use the relation that q propto sqrt(b)
      float bValue = (this->getBValues() )[ s*_numberOfOrientationsPerShell ];
      _qValues.push_back( std::sqrt( bValue / 1000 ) / (2*M_PI) );
      
      if( s > 0 && ( _qValues[ s - 1 ] == _qValues[ s ] ) ) 
      {
	
	throw std::runtime_error(
			  "Error in  b values or orientations per shell \
with respect to DW data inputed" );

      }

      if( verbose ) 
      {
	
	std::cout << " " << _qValues[ s ] << "/" << bValue << " ";

      }

    }
    if( verbose ) 
    {
	
      std::cout << "]\n";
	
    }
    
    for( s = 0; s < _numberOfShells; s++)
    {

      j = 0;

      IrregularSolidHarmonicCoefficients[ s ].reallocate( N, N );
      RegularSolidHarmonicCoefficients[ s ].reallocate( N, N );

      for( n = 0; n <= _maximumSphericalHarmonicOrder; n+=2) 
      {
	
	for( m = -n; m <= n; m++) 
	{
	  
	  if(n == 0) 
	  {

	    if ( _qValues [ s ] == 0 ) 
	    {
	      
	      throw std::runtime_error( "qValue is null : division by zero" );

	    }
	    else
	    {
	      
	      IrregularSolidHarmonicCoefficients[ s ]( j, j ) = 1 / _qValues[ s ]; 

	    }
	    
	    RegularSolidHarmonicCoefficients[ s ]( j, j ) = 1;    
	  
	  }
	  else 
	  {

	    if ( _qValues[ s ] == 0 ) 
	    {
	      
	      throw std::runtime_error( "qValue is null : division by zero" );

	    }
	    else
	    {
	    
	      IrregularSolidHarmonicCoefficients[ s ]( j, j ) = 
		1 / std::pow( _qValues[ s ], n + 1 ); 
	      
	    }

	    RegularSolidHarmonicCoefficients[ s ]( j, j )  = 
	      std::pow( _qValues[ s ], n );     

	  }
	  
	  j++;

	}

      }

    }      

    gkg::Matrix IrregularSolidHarmonicMatrices[ _numberOfShells ];
    gkg::Matrix RegularSolidHarmonicMatrices[ _numberOfShells ];

    for( s = 0; s < _numberOfShells; s++ ) 
    {
      
      IrregularSolidHarmonicMatrices[ s ] = B.getComposition(
					 IrregularSolidHarmonicCoefficients[ s ] );

      RegularSolidHarmonicMatrices[ s ] = B.getComposition(
					 RegularSolidHarmonicCoefficients[ s ] );

    }

    // Descoteaux et al MRM 2009, Eq. [10]
    // Laplacian Assumption for the signal
    // E(q) = sum_j [ c_j / q^(l(j)+1) + d_j q^(l(j)) ] Y (theta, phi)

    // A = [ Irr1*B , Reg1*B
    //       Irr2*B , Reg2*B
    //       ...
    //       Irrn*B , Regn*B ]
    gkg::Matrix A( 2*N , _numberOfShells*M );
    for( n = 0; n < A.getSize1(); n++ ) 
    {
      
      s = 1;
      for( m = 0; m < A.getSize2(); m++ ) 
      {      
	  
	if( m / s == M )
	  s++;

	//std::cout << "n,m : " << n << "," << m << "\t";
	if( n < N) 
	{ // irregural coefficients: first half of the A matrix
	  
	  //std::cout << "first : " << m - (s-1)*M << "," << n << "\n";
	  A( n, m ) = IrregularSolidHarmonicMatrices[ s-1 ]( m - (s-1)*M, n );	  
	  
	}
	else 
	{  // regural coefficients: 2nd half of the B matrix
	    
	  //std::cout << "second : " << m - (s-1)*M << "," << n - N << "\n";
	  A( n, m ) = RegularSolidHarmonicMatrices[ s-1 ]( m - (s-1)*M, n - N );
	  
	}
	
      }
      
    }

    // building reconstruction matrix Aplus
    // reconstructionMatrix = (A^T A)^(-1) A^T
    _reconstructionMatrix.reallocate( A.getSize2(), A.getSize1() );
    if( verbose )
    {
    
      std::cout << "reconstructionMatrix : " << _reconstructionMatrix.getSize1() 
		<< "x" << _reconstructionMatrix.getSize2() << "\n";

    }
    
    double lambda = 1.0;
    //factory->getMoorePenrosePseudoInverse( A, _reconstructionMatrix );    
    factory->getThikonovPseudoInverse( A.getTransposition(), L,
    				       lambda,
    				       _reconstructionMatrix );
    _reconstructionMatrix = _reconstructionMatrix.getTransposition();
    
    
    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::DpiEapCartesianField::getReconstructionMatrix( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::DpiEapCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DpiEap dpiEap( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_qValues,
			_reconstructionMatrix );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dpiEap );

    }
    else
    {

      return dpiEap;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DpiEapCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::DpiEapCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DpiEap dpiEap( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_qValues,
			_reconstructionMatrix );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dpiEap );

    }
    else
    {

      return dpiEap;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DpiEapCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
